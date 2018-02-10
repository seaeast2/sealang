
Git command 

1. Git Clone : Downloads files in git repository.
  git clone https://github.com/blarblar/ ~/test/dir

2. Git Commit : Updates changes in LOCAL repository.
  git commit -am "message"

3. push : Synchronize repository between local and server.
  git push origin master

4. Add file
  git add filename
  git commit -am "Added new filename"
  git push origin master

5. Pull file. Update files with latest revision. (This is same as 'update' command in SVN.)
  git pull

6. Delete file
  git rm filename
  git commit -am "Delete filename"
  git push origin master
