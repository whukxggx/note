1. **内存布局**

   [内存布局图]()

   图中，栈至顶向下扩展，堆至底向上扩展，mmap映射区域自顶向下扩展。即mmap映射区域和堆相对扩展，直至耗尽虚拟地址空间的剩余区域，这种结构便于c运行时库使用mmap映射区域和堆进行内存分配。

2. **brk（sbrk）和mmap函数**

   * brk和sbrk

     ```c
     #include<unistd.h>
     int brk(const void *addr);
     void *sbrk(intptr_t incr);
     ```

     两者的作用是扩展heap的上界brk；

     brk的参数**设置为新的brk上界地址**，**成功返回1**，失败返回0；

     sbrk的参数**为申请内存的大小**，返回**heap新的上界brk的地址。**

   * mmap()

     ```c
     #include<sys/mman.h>
     void *mmap(void *addr,size_t length,int prot,int flags,int fd,off_t offset);
     int munmap(void *addr,size_t length);
     ```

   ​       mmap的**第一种用法是映射此磁盘文件到内存中**，**第二种用法是匿名映射，不映射磁盘文件，而向映射区申请一块内存**

   ​		**malloc使用的是mmap的第二种用法**

   ​		**munmap用于释放内存**

3. **主分配区和非主分配区**

   allocate的内存分配区中，**为了解决多线程锁争夺问题，分为主分配区main_area和非主分配区no_main_area**：

   1. 主分配区和非主分配区形成一个环形链表进行管理。
   2. 每一个分配区利用互斥锁使线程对于该分配区的访问互斥。
   3. 每个进程只有一个主分配区，可以有多个非主分配区。
   4. ptmalloc根据系统对分配区的争用动态增加分配区的大小，分配区的数量一旦增加，则不会减少。
   5. 主分配区可以使用brk喝mmap来分配，而非主分配区只能使用mmap来映射内存。
   6. 申请小内存时会产生很多内存碎片，ptmalloc在整理时也需要对分配区做加锁操作。

   **当一个线程需要使用malloc分配内存的时候，会先查看该线程的私有变量中是否已经存在一个分配区。若是存在。会尝试对其进行加锁操作。若是加锁成功，就在使用该分配区分配内存，若是失败，就会遍历循环链表中获取一个未加锁的分配区。若是整个链表中都没有未加锁的分配区，则malloc会开辟一个新的分配区，将其加入全局的循环链表并加锁，然后使用该分配区进行内存分配。当释放这块内存时，同样会先获取待释放内存块所在的分配区的锁。若是有其他线程正在使用该分配区，则必须等待其他线程释放该分配区互斥锁之后才能进行释放内存的操作。**

4. **malloc实现原理**

   　因为brk、sbrk、mmap都属于系统调用，若每次申请内存，都调用这三个，那么每次都会产生系统调用，影响性能；其次，这样申请的内存容易产生碎片，因为堆是从低地址到高地址，如果高地址的内存没有被释放，低地址的内存就不能被回收。

   **所以malloc采用的是内存池的管理方式（ptmalloc）**，ptmalloc使用边界标记法将内存划分为很多块，从而对内存的分配和回收进行管理。为了内存分配函数malloc的高效性，ptmalloc会预先向操作系统申请一块内存供用户使用，当我们申请和释放内存的时候，ptmalloc会将这些内存管理起来，并通过一些策略来判断是否将其回收给操作系统。这样做的最大好处就是，使用户申请和释放内存的时候更加高效，避免产生过多的内存碎片。

5. **内存分配malloc流程**
   1. 获取分配区的锁，防止多线程冲突。
   2. 计算出实际需要分配的内存的chunk实际大小。
   3. 判断chunk的大小，如果小于max_fast（64Ｂ），则尝试去fast bins上取适合的chunk，如果有则分配结束。否则，下一步；
   4. 判断chunk大小是否小于512B，如果是，则从small bins上去查找chunk，如果有合适的，则分配结束。否则下一步；
   5. ptmalloc首先会遍历fast bins中的chunk，将相邻的chunk进行合并，并链接到unsorted bin中然后遍历 unsorted bins。如果unsorted bins上只有一个chunk并且大于待分配的chunk，则进行切割，并且剩余的chunk继续扔回unsorted bins；如果unsorted bins上有大小和待分配chunk相等的，则返回，并从unsorted bins删除；如果unsorted bins中的某一chunk大小 属于small bins的范围，则放入small bins的头部；如果unsorted bins中的某一chunk大小 属于large bins的范围，则找到合适的位置放入。若未分配成功，转入下一步；
   6. 从large bins中查找找到合适的chunk之后，然后进行切割，一部分分配给用户，剩下的放入unsorted bin中。
   7. 如果搜索fast bins和bins都没有找到合适的chunk，那么就需要操作top chunk来进行分配了
      当top chunk大小比用户所请求大小还大的时候，top chunk会分为两个部分：User chunk（用户请求大小）和Remainder chunk（剩余大小）。其中Remainder chunk成为新的top chunk。
      　　当top chunk大小小于用户所请求的大小时，top chunk就通过sbrk（main arena）或mmap（thread arena）系统调用来扩容。
   8. 到了这一步，说明 top chunk 也不能满足分配要求，所以，于是就有了两个选择: 如 果是主分配区，调用 sbrk()，增加 top chunk 大小；如果是非主分配区，调用 mmap 来分配一个新的 sub-heap，增加 top chunk 大小；或者使用 mmap()来直接分配。在 这里，需要依靠 chunk 的大小来决定到底使用哪种方法。判断所需分配的 chunk 大小是否大于等于 mmap 分配阈值，如果是的话，则转下一步，调用 mmap 分配， 否则跳到第 10 步，增加 top chunk 的大小。
   9. 使用 mmap 系统调用为程序的内存空间映射一块 chunk_size align 4kB 大小的空间。 然后将内存指针返回给用户。
   10. 判断是否为第一次调用 malloc，若是主分配区，则需要进行一次初始化工作，分配 一块大小为(chunk_size + 128KB) align 4KB 大小的空间作为初始的 heap。若已经初 始化过了，主分配区则调用 sbrk()增加 heap 空间，分主分配区则在 top chunk 中切 割出一个 chunk，使之满足分配需求，并将内存指针返回给用户。
       简而言之： 获取分配区(arena)并加锁–> fast bin –> unsorted bin –> small bin –> large bin –> top chunk –> 扩展堆

* **内存回收流程**
  1. 获取分配区的锁，保证线程安全。
  2. 如果free的是空指针，则返回，什么都不做。
  3. 判断当前chunk是否是mmap映射区域映射的内存，如果是，则直接munmap()释放这块内存。前面的已使用chunk的数据结构中，我们可以看到有M来标识是否是mmap映射的内存。
  4. 判断chunk是否与top chunk相邻，如果相邻，则直接和top chunk合并（和top chunk相邻相当于和分配区中的空闲内存块相邻）。转到步骤8
  5. 如果chunk的大小大于max_fast（64b），则放入unsorted bin，并且检查是否有合并，有合并情况并且和top chunk相邻，则转到步骤8；没有合并情况则free。
  6. 如果chunk的大小小于 max_fast（64b），则直接放入fast bin，fast bin并没有改变chunk的状态。没有合并情况，则free；有合并情况，转到步骤7
  7. 在fast bin，如果当前chunk的下一个chunk也是空闲的，则将这两个chunk合并，放入unsorted bin上面。合并后的大小如果大于64B，会触发进行fast bins的合并操作，fast bins中的chunk将被遍历，并与相邻的空闲chunk进行合并，合并后的chunk会被放到unsorted bin中，fast bin会变为空。合并后的chunk和topchunk相邻，则会合并到topchunk中。转到步骤8
  8. 判断top chunk的大小是否大于mmap收缩阈值（默认为128KB），如果是的话，对于主分配区，则会试图归还top chunk中的一部分给操作系统。free结束。

* **转载并修改自：**

  [malloc原理](https://blog.csdn.net/z_ryan/article/details/79950737)

  