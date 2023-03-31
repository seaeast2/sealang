#include "Common.h"

#include <stack>
#include <unordered_map>
#include <unordered_set>



class Inst
{
public:
    std::vector<Inst*> Uses; // 보통 1~3개
    std::vector<Inst*> Defs; // 보통 1개
};


// SSA 의사코드
class BasicBlock
{
public:
    // CFG 상에서
    std::vector<BasicBlock*> pred; // 이 노드로 들어오는 모든 노드들의 집합
    std::vector<BasicBlock*> succ; // 이 노드 밖으로 나가는 모든 노드들의 집합

    // 지배자 tree 와 상관 없이 자식으로 등록된 노드
    std::vector<BasicBlock*> children;

    std::vector<Inst*> Stmts; // 현 BB의 모든 statment들의 집합
    std::vector<Inst*> Aorig; // def 집합
    std::unordered_map<Inst*, PhiInst*> Phis; // phi 집합

    // vector 의 index 계산
    int getIndex(vector<BasicBlock*> v, BasicBlock* K)
    {
        auto it = find(v.begin(), v.end(), K);

        // If element was found
        if (it != v.end())
        {

            // calculating the index
            // of K
            int index = it - v.begin();
            //cout << index << endl;
            return index;
        }
        else {
            // If the element is not
            // present in the vector
            //cout << "-1" << endl;
            return -1;
        }
    }
};


class SSA
{
public:
    int N;
    // <node, dfnum> : node 로 해당 dfnum 얻기
    std::unordered_map<BasicBlock*, int> dfnum;
    // <dfnum, node> : dfnum 으로 해당 node 얻기
    std::unordered_map<int, BasicBlock*> vertex;
    // <child, parent> : 자식 node 로 부모 node 참조. 부모가 여러개일 경우는??
    std::unordered_map<BasicBlock*, BasicBlock*> parent;

    // <semidominator, node> : 준지배자 숲. 준지배자에게 지배받는 모든 노드들의 집합. (노드중 준지배자가 같은 노드들의 집합.)
    std::unordered_map<BasicBlock*, std::vector<BasicBlock*>> bucket; 
    // <node, semidominator> : 특정 노드의 준지배자 얻기
    std::unordered_map<BasicBlock*, BasicBlock*> semi;
    // 신장 숲에서 v 위의 모든 노드.
    std::unordered_map<BasicBlock*, BasicBlock*> ancestor;
    
    std::unordered_map<BasicBlock*, BasicBlock*> samedom;
    // <node, semidominator> : 준지배자가 ancestor[v]에서 v까지 건너뛴 경로에서 가장 낮은 dfnum을 갖는 노드(v는 포함하지만 ancestor[v]는 제외).
    std::unordered_map<BasicBlock*, BasicBlock*> best;

    //= Modern Compiler Implementation의 SSA 알고리즘 구현 =============
    // 함수 1개에 들어 있는 모든 BB 들의 집합
    std::vector<BasicBlock*> BBs;
    // <node, idom> : node 에 대한 idom 노드를 반환
    std::unordered_map<BasicBlock*, BasicBlock*> idom;
    // <idom, idom's child> : 직접 지배하는 자식 집합
    std::unordered_map<BasicBlock*, std::vector<BasicBlock*>> children;
    // 노드 n의 지배 경계 집합
    std::unordered_map<BasicBlock*, std::vector<BasicBlock*>> DF;
    // 변수 a 에대한 phi 함수를 가져야 하는 노드들의 집합
    std::unordered_map<BasicBlock*, std::unordered_set<Inst*>> Aphi;
    // 변수 a가 정의된 BB 들의 집합
    std::unordered_map<Inst*, std::vector<BasicBlock*>> defsites;
    // 변수 a 의 재정의 횟수
    std::unordered_map<Inst*, int> Count;
    // 변수 a의 stack
    std::unordered_map<Inst*, std::stack<int>> Stack;


public:
    // 지배 경계를 계산
    void computeDF(BasicBlock* n);
    // dominator가 dominated 를 지배하는지 여부를 반환
    bool IsDomitanted(BasicBlock* dominator, BasicBlock* dominated);
    // Phi 함수 삽입
    void PlacePhiFunctions();
    // 변수 이름 변경
    void Initialization();
    
    void Rename(BasicBlock* n);

    // LENGAUER-TARJAN ALGORITHM 위한 선행 Dfs 함수
    void Dfs(BasicBlock* p, BasicBlock* n);
    // LENGAUER-TARJAN ALGORITHM 
    void Dominators(BasicBlock* r); // r 은 root basicblock

    void Link1(BasicBlock* p, BasicBlock* n);
    void Link2(BasicBlock* p, BasicBlock* n);
    // v의 가장 작은 dfnum 을 갖는 준지배자를 반환 (일반 버전)
    BasicBlock* AncestorWithLowestSemi1(BasicBlock* v);
    // v의 가장 작은 dfnum 을 갖는 준지배자를 반환 (경로 압축 버전)
    BasicBlock* AncestorWithLowestSemi2(BasicBlock* v);

};

// 지배 경계를 계산
void SSA::computeDF(BasicBlock* n)
{
    std::vector<BasicBlock*> S;

    // 이곳에서 DFlocal[n]을 계산한다.
    for (auto y : n->succ)
    {
        if (idom[y] != n)
            S.push_back(y);
    }

    // idom 이 n 인 자식 노드들의 집합 순회
    for (auto c : children[n])
    {
        computeDF(c);
        // 이곳에서 DFup[c] 를 계산한다.
        for (auto w : DF[n])
        {
            if (IsDomitanted(n, w) == false || n == w)
                S.push_back(w);
        }
    }

    DF[n] = S;
}

bool SSA::IsDomitanted(BasicBlock* dominator, BasicBlock* dominated)
{
    if (idom[dominated] == nullptr)
        return false;

    if (idom[dominated] != dominator)
        return IsDomitanted(dominator, idom[dominated]);

    return true;
}

void SSA::PlacePhiFunctions()
{
    // 모든 변수를 순회하면서 defsites 구축
    for (auto n : BBs)
    {// 모든 BB 순회
        for (auto a : n->Aorig) // n에서 정의된 모든 변수 순회
        {
            defsites[a].push_back(n);
        }
    }

    // 모든 변수를 순회
    for (auto n : BBs)
    {
        for (auto a : n->Aorig)
        {
            auto W = defsites[a];
            while (!W.empty())
            {// 변수 a가 정의된 노드들을 순회
                auto nn = W.back();
                W.pop_back();
                for (auto y : DF[nn])
                { // 변수 a가 정의된 노드들의 지배 경계를 순회한다.

                    if (Aphi[y].find(a) == Aphi[y].end())
                    { // 지배경계 노드 y 에 a 가 phi 함수에 등록되어 있지 않은 경우

                        // Phis 의 second 가 PhiInst 이고, AddPhiArg() 란 멤버 함수를 가지고 있다고 가정한다.
                        y->Phis[a]->AddPhiArg(a);

                        // 지배경계 노드 y 의 phi 함수 추가 변수 목록에 a를 등록한다.
                        Aphi[y].insert(a);

                        if (std::find(y->Aorig.begin(), y->Aorig.end(), a) == y->Aorig.end())
                        {
                            // phi 함수는 변수 정의와 동일한 효과를 내므로
                            // 변수 a가 노드 y 에서 정의되지 않았으면, 추가한다.
                            W.push_back(y);
                            y->Aorig.push_back(a);
                        }
                    }
                }
            }
        }
    }
}

void SSA::Initialization()
{
    // Rename 을 위한 초기화
    // 모든 변수를 순회
    for (auto n : BBs)
    {
        for (auto a : n->Aorig)
        {
            Count[a] = 0;
            while (!Stack[a].empty()) 
                Stack[a].pop(); // std::stack<> 은 clear 함수를 지원하지 않음.
            Stack[a].push(0);
        }
    }
}

void SSA::Rename(BasicBlock* n)
{
    for (auto S : n->Stmts)
    {
        // Use 부분 이름 변경
        if (S->IsPhiFunc() == false)
        { // S가 phi 함수가 아니면
            for (auto x : S->Uses)
            { // S 안에서 사용 목록 x 순회
                int i = Stack[x].top();
                Inst* xi = new Inst(x, i); // i 를 phi 명으로 갖는 변수 복사 생성
                x->replace(xi); // x 를 xi 로 교체
            }
        }
        // Def 부분 이름 변경
        for (auto a : S->Defs)
        {
            Count[a] += 1;
            int i = Count[a];
            Stack[a].push(i);

            Inst* ai = new Inst(a, i); // i 를 phi 명으로 갖는 변수 복사 생성
            a->replace(ai); // x 를 xi 로 교체
        }
    }

    // 노드의 phi 함수들을 순회하면서 phi 함수의 arg 의 이름 변경
    for (auto Y : n->succ)
    {
        // n 의 Y 안에서의 predecessor index 를 구한다.
        int j = Y->getIndex(Y->pred, n);

        for (auto phi : Y->Phis)
        {
            auto a = phi.second->GetPhiArg(j); // PhiInst::GetPhiArg(int j) 는 j 번째 phi arg 를 반환.
            int i = Stack[a].top();
            phi.second->ReplacePhiArg(j, new Inst(a, i)); // PhiInst::ReplacePhiArg(int j, Inst* inst) 는 j 번재 phi arg 를 inst 로 치환
        }
    }

    for (auto X : n->children)
    {
        Rename(X);
    }

    for (auto S : n->Stmts)
    {
        for (auto a : S->Defs)
        {
            Stack[a].pop();
        }
    }
}

void SSA::Dfs(BasicBlock* p, BasicBlock* n)
{
    if (dfnum[n] == 0)
    {
        dfnum[n] = N; // defnum 숫자 설정
        vertex[N] = n; // defnum 의 역참조 설정
        parent[n] = p;

        N = N + 1;

        for (auto w : n.succ)
        {
            Dfs(n, w);
        }
    }
}

void SSA::Dominators(BasicBlock* r)
{
    N = 0;
    // 초기화
    // bucket, dfnum, semi, ancestor, idom, samedom 초기화
    Dfs(nullptr, r);

    for (int i = N - 1; i; i--) // Skip over node 0, the root node.
    {
        BasicBlock  *n, // 현재 노드
                    *p, // 부모 노드
                    *s; // 준지배자
        
        n = vertex[i]; // dfnum 에 해당하는 node 얻기
        p = parent[n]; // 부모 노드 얻기
        s = p; // 준지배자
        
        // 준지배자 정리를 통해 n의 준지배자를 계산한다.
        for (auto v : n->pred)
        {
            BasicBlock *ss; // 준지배자 후보
            
            if (dfnum[v] <= dfnum[n])
            { // v 가 n 의 적절한 조상일 때
                ss = v;
            }
            else
            {
                ss = semi[AncestorWithLowestSemi1(v)];
            }
            
            if (dfnum[ss] < dfnum[s])
                s = ss;
        }
        
        semi[n] = s; // 현재 노드의 준지배자를 설정
        bucket[s].push_back(n); // 준지배자 s에 의해 지배되는 모든 노드 n을 추가
        
        // n의 지배자 계산은 s -> n 이 숲에서 연결될 때까지 미뤄진다.
        Link1(p, n);
        
        // 이제 p에서 v까지의 경로가 신장 숲에 연결되었으므로 
        // 이러한 선은 지배자 정리의 첫 번째 절을 기반으로 v의 
        // 지배자를 계산하거나 y의 지배자가 알려질 때까지 계산을 연기합니다.
        for (auto v : bucket[p]) 
        {// n의 부모 p를 준지배자로 갖는 모든 노드 v
            BasicBlock* y = AncestorWithLowestSemi1(v);
            if (semi[y] == semi[v])
            { // y 와 v의 준지배자가 같으면
                idom[v] = p; // v의 직접 지배자를 p로 설정
            }
            else
            {
                samedom[v] = y;
            }
        }
        
        bucket[p].clear();
    }
    
    for (int i = 1; i < N -1; i++)
    {
        BasicBlock  *n = vertex[i];
        if (samedom[n] != nullptr)
        {
            idom[n] = idom[samedom[n]];
        }
    }
}

BasicBlock* SSA::AncestorWithLowestSemi1(BasicBlock* v)
{ // 느린 버전 O(n)
    BasicBlock* u = v;
    
    // 
    while(ancestor[v] != nullptr)
    {
        if (dfnum[semi[v]] < dfnum[semi[u]])
        {
            u = v;
        }
        
        v = ancestor[v];
    }
    
    return u;
}

void SSA::Link1(BasicBlock* p, BasicBlock* n)
{
    ancestor[n] = p;
}


BasicBlock* SSA::AncestorWithLowestSemi2(BasicBlock* v)
{ // 빠른번전 O(log n)
    BasicBlock* a = ancestor[v];
    if (ancestor[a] != nullptr)
    {
        BasicBlock* b = AncestorWithLowestSemi2(a);
        ancestor[v] = ancestor[a];
        if (dfnum[semi[b]] < dfnum[semi[best[v]]])
        {
            best[v] = b;
        }
    }
    
    return best[v];
}

void SSA::Link2(BasicBlock* p, BasicBlock* n)
{
    ancestor[n] = p;
    best[n] = n;
}