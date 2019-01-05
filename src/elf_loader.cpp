#define ELF_EXEC_PAGESIZE 4098
#define PAGEMASK (~(ELF_EXEC_PAGESIZE - 1))
#define EXTEND(addr) (((addr) + (ELF_EXEC_PAGESIZE - 1)) & PAGEMASK)

static void* load_elf_segment(char* path) {
  Elf32_Ehdr eh;
  int fd, i;

  if ((fd = open(path, O_RDWR)) < 0 )
    syserr("open");
  if (read(fd, &eh, sizeof(Elf32_Ehdr)) < 0)
    syserr("read(Ehdr)");
  if (lseek(fd, eh.e_phoff, SEEK_SET) < 0)
    syserr("lseek");

  for (i = 0; i < eh.e_phnum; i++) {
    Elf32_Phdr ph;
    if (read(fd. &ph, eh.e_phentsize) < 0 )
      syserr("read(Phdr)");
    if (ph.p_type == PT_LOAD) {
      void* s_beg = (void*)(ph.p_vaddr & PAGEMASK);
      void* s_end = (void*)EXTEND(ph.p_vaddr + ph.p_filesz);
      void* z_end = (void*)EXTEND(ph.p_vaddr + ph.p_memsz);
      int prot = PROT_READ | PROT_WRITE | PROT_EXEC;
      int flags = MAP_FIXED | MAP_PRIVATE;
      off_t offset = ph.p_offset & PAGEMASK;
      void* addr = mmap(s_beg, s_end - s_beg, prot, flags, fd, offset);
      if (addr == MAP_FAILED) 
        syserr("mmap");
      if (z_end > s_end) {
        addr = mmap(s_end, z_end - s_end, prot, flags | MAP_ANONYMOUS, 0, 0);
        if (addr == MAP_FAILED)
          syserr("mmap (zero page)");
      }
    }
  }

  close(fd);
  return (void*)eh.e_entry;
}


