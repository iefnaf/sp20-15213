
echo:     file format elf64-x86-64


Disassembly of section .init:

00000000000005c0 <_init>:
 5c0:	48 83 ec 08          	sub    $0x8,%rsp
 5c4:	48 8b 05 1d 0a 20 00 	mov    0x200a1d(%rip),%rax        # 200fe8 <__gmon_start__>
 5cb:	48 85 c0             	test   %rax,%rax
 5ce:	74 02                	je     5d2 <_init+0x12>
 5d0:	ff d0                	callq  *%rax
 5d2:	48 83 c4 08          	add    $0x8,%rsp
 5d6:	c3                   	retq   

Disassembly of section .plt:

00000000000005e0 <.plt>:
 5e0:	ff 35 ca 09 20 00    	pushq  0x2009ca(%rip)        # 200fb0 <_GLOBAL_OFFSET_TABLE_+0x8>
 5e6:	ff 25 cc 09 20 00    	jmpq   *0x2009cc(%rip)        # 200fb8 <_GLOBAL_OFFSET_TABLE_+0x10>
 5ec:	0f 1f 40 00          	nopl   0x0(%rax)

00000000000005f0 <puts@plt>:
 5f0:	ff 25 ca 09 20 00    	jmpq   *0x2009ca(%rip)        # 200fc0 <puts@GLIBC_2.2.5>
 5f6:	68 00 00 00 00       	pushq  $0x0
 5fb:	e9 e0 ff ff ff       	jmpq   5e0 <.plt>

0000000000000600 <_IO_getc@plt>:
 600:	ff 25 c2 09 20 00    	jmpq   *0x2009c2(%rip)        # 200fc8 <_IO_getc@GLIBC_2.2.5>
 606:	68 01 00 00 00       	pushq  $0x1
 60b:	e9 d0 ff ff ff       	jmpq   5e0 <.plt>

0000000000000610 <exit@plt>:
 610:	ff 25 ba 09 20 00    	jmpq   *0x2009ba(%rip)        # 200fd0 <exit@GLIBC_2.2.5>
 616:	68 02 00 00 00       	pushq  $0x2
 61b:	e9 c0 ff ff ff       	jmpq   5e0 <.plt>

Disassembly of section .plt.got:

0000000000000620 <__cxa_finalize@plt>:
 620:	ff 25 d2 09 20 00    	jmpq   *0x2009d2(%rip)        # 200ff8 <__cxa_finalize@GLIBC_2.2.5>
 626:	66 90                	xchg   %ax,%ax

Disassembly of section .text:

0000000000000630 <_start>:
 630:	31 ed                	xor    %ebp,%ebp
 632:	49 89 d1             	mov    %rdx,%r9
 635:	5e                   	pop    %rsi
 636:	48 89 e2             	mov    %rsp,%rdx
 639:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
 63d:	50                   	push   %rax
 63e:	54                   	push   %rsp
 63f:	4c 8d 05 0a 02 00 00 	lea    0x20a(%rip),%r8        # 850 <__libc_csu_fini>
 646:	48 8d 0d 93 01 00 00 	lea    0x193(%rip),%rcx        # 7e0 <__libc_csu_init>
 64d:	48 8d 3d 69 01 00 00 	lea    0x169(%rip),%rdi        # 7bd <main>
 654:	ff 15 86 09 20 00    	callq  *0x200986(%rip)        # 200fe0 <__libc_start_main@GLIBC_2.2.5>
 65a:	f4                   	hlt    
 65b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

0000000000000660 <deregister_tm_clones>:
 660:	48 8d 3d a9 09 20 00 	lea    0x2009a9(%rip),%rdi        # 201010 <stdin@@GLIBC_2.2.5>
 667:	55                   	push   %rbp
 668:	48 8d 05 a1 09 20 00 	lea    0x2009a1(%rip),%rax        # 201010 <stdin@@GLIBC_2.2.5>
 66f:	48 39 f8             	cmp    %rdi,%rax
 672:	48 89 e5             	mov    %rsp,%rbp
 675:	74 19                	je     690 <deregister_tm_clones+0x30>
 677:	48 8b 05 5a 09 20 00 	mov    0x20095a(%rip),%rax        # 200fd8 <_ITM_deregisterTMCloneTable>
 67e:	48 85 c0             	test   %rax,%rax
 681:	74 0d                	je     690 <deregister_tm_clones+0x30>
 683:	5d                   	pop    %rbp
 684:	ff e0                	jmpq   *%rax
 686:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 68d:	00 00 00 
 690:	5d                   	pop    %rbp
 691:	c3                   	retq   
 692:	0f 1f 40 00          	nopl   0x0(%rax)
 696:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 69d:	00 00 00 

00000000000006a0 <register_tm_clones>:
 6a0:	48 8d 3d 69 09 20 00 	lea    0x200969(%rip),%rdi        # 201010 <stdin@@GLIBC_2.2.5>
 6a7:	48 8d 35 62 09 20 00 	lea    0x200962(%rip),%rsi        # 201010 <stdin@@GLIBC_2.2.5>
 6ae:	55                   	push   %rbp
 6af:	48 29 fe             	sub    %rdi,%rsi
 6b2:	48 89 e5             	mov    %rsp,%rbp
 6b5:	48 c1 fe 03          	sar    $0x3,%rsi
 6b9:	48 89 f0             	mov    %rsi,%rax
 6bc:	48 c1 e8 3f          	shr    $0x3f,%rax
 6c0:	48 01 c6             	add    %rax,%rsi
 6c3:	48 d1 fe             	sar    %rsi
 6c6:	74 18                	je     6e0 <register_tm_clones+0x40>
 6c8:	48 8b 05 21 09 20 00 	mov    0x200921(%rip),%rax        # 200ff0 <_ITM_registerTMCloneTable>
 6cf:	48 85 c0             	test   %rax,%rax
 6d2:	74 0c                	je     6e0 <register_tm_clones+0x40>
 6d4:	5d                   	pop    %rbp
 6d5:	ff e0                	jmpq   *%rax
 6d7:	66 0f 1f 84 00 00 00 	nopw   0x0(%rax,%rax,1)
 6de:	00 00 
 6e0:	5d                   	pop    %rbp
 6e1:	c3                   	retq   
 6e2:	0f 1f 40 00          	nopl   0x0(%rax)
 6e6:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 6ed:	00 00 00 

00000000000006f0 <__do_global_dtors_aux>:
 6f0:	80 3d 21 09 20 00 00 	cmpb   $0x0,0x200921(%rip)        # 201018 <completed.7698>
 6f7:	75 2f                	jne    728 <__do_global_dtors_aux+0x38>
 6f9:	48 83 3d f7 08 20 00 	cmpq   $0x0,0x2008f7(%rip)        # 200ff8 <__cxa_finalize@GLIBC_2.2.5>
 700:	00 
 701:	55                   	push   %rbp
 702:	48 89 e5             	mov    %rsp,%rbp
 705:	74 0c                	je     713 <__do_global_dtors_aux+0x23>
 707:	48 8b 3d fa 08 20 00 	mov    0x2008fa(%rip),%rdi        # 201008 <__dso_handle>
 70e:	e8 0d ff ff ff       	callq  620 <__cxa_finalize@plt>
 713:	e8 48 ff ff ff       	callq  660 <deregister_tm_clones>
 718:	c6 05 f9 08 20 00 01 	movb   $0x1,0x2008f9(%rip)        # 201018 <completed.7698>
 71f:	5d                   	pop    %rbp
 720:	c3                   	retq   
 721:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
 728:	f3 c3                	repz retq 
 72a:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

0000000000000730 <frame_dummy>:
 730:	55                   	push   %rbp
 731:	48 89 e5             	mov    %rsp,%rbp
 734:	5d                   	pop    %rbp
 735:	e9 66 ff ff ff       	jmpq   6a0 <register_tm_clones>

000000000000073a <smash>:
 73a:	48 83 ec 08          	sub    $0x8,%rsp
 73e:	48 8d 3d 1f 01 00 00 	lea    0x11f(%rip),%rdi        # 864 <_IO_stdin_used+0x4>
 745:	e8 a6 fe ff ff       	callq  5f0 <puts@plt>
 74a:	bf 00 00 00 00       	mov    $0x0,%edi
 74f:	e8 bc fe ff ff       	callq  610 <exit@plt>

0000000000000754 <gets>:
 754:	55                   	push   %rbp
 755:	53                   	push   %rbx
 756:	48 83 ec 08          	sub    $0x8,%rsp
 75a:	48 89 fd             	mov    %rdi,%rbp
 75d:	48 89 fb             	mov    %rdi,%rbx
 760:	eb 07                	jmp    769 <gets+0x15>
 762:	48 83 c3 01          	add    $0x1,%rbx
 766:	88 43 ff             	mov    %al,-0x1(%rbx)
 769:	48 8b 3d a0 08 20 00 	mov    0x2008a0(%rip),%rdi        # 201010 <stdin@@GLIBC_2.2.5>
 770:	e8 8b fe ff ff       	callq  600 <_IO_getc@plt>
 775:	83 f8 ff             	cmp    $0xffffffff,%eax
 778:	74 05                	je     77f <gets+0x2b>
 77a:	83 f8 0a             	cmp    $0xa,%eax
 77d:	75 e3                	jne    762 <gets+0xe>
 77f:	83 f8 ff             	cmp    $0xffffffff,%eax
 782:	75 05                	jne    789 <gets+0x35>
 784:	48 39 eb             	cmp    %rbp,%rbx
 787:	74 0d                	je     796 <gets+0x42>
 789:	c6 03 00             	movb   $0x0,(%rbx)
 78c:	48 89 e8             	mov    %rbp,%rax
 78f:	48 83 c4 08          	add    $0x8,%rsp
 793:	5b                   	pop    %rbx
 794:	5d                   	pop    %rbp
 795:	c3                   	retq   
 796:	b8 00 00 00 00       	mov    $0x0,%eax
 79b:	eb f2                	jmp    78f <gets+0x3b>

000000000000079d <echo>:
 79d:	53                   	push   %rbx
 79e:	48 83 ec 10          	sub    $0x10,%rsp
 7a2:	48 8d 5c 24 0c       	lea    0xc(%rsp),%rbx
 7a7:	48 89 df             	mov    %rbx,%rdi
 7aa:	e8 a5 ff ff ff       	callq  754 <gets>
 7af:	48 89 df             	mov    %rbx,%rdi
 7b2:	e8 39 fe ff ff       	callq  5f0 <puts@plt>
 7b7:	48 83 c4 10          	add    $0x10,%rsp
 7bb:	5b                   	pop    %rbx
 7bc:	c3                   	retq   

00000000000007bd <main>:
 7bd:	48 83 ec 08          	sub    $0x8,%rsp
 7c1:	b8 00 00 00 00       	mov    $0x0,%eax
 7c6:	e8 d2 ff ff ff       	callq  79d <echo>
 7cb:	b8 00 00 00 00       	mov    $0x0,%eax
 7d0:	48 83 c4 08          	add    $0x8,%rsp
 7d4:	c3                   	retq   
 7d5:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 7dc:	00 00 00 
 7df:	90                   	nop

00000000000007e0 <__libc_csu_init>:
 7e0:	41 57                	push   %r15
 7e2:	41 56                	push   %r14
 7e4:	49 89 d7             	mov    %rdx,%r15
 7e7:	41 55                	push   %r13
 7e9:	41 54                	push   %r12
 7eb:	4c 8d 25 b6 05 20 00 	lea    0x2005b6(%rip),%r12        # 200da8 <__frame_dummy_init_array_entry>
 7f2:	55                   	push   %rbp
 7f3:	48 8d 2d b6 05 20 00 	lea    0x2005b6(%rip),%rbp        # 200db0 <__init_array_end>
 7fa:	53                   	push   %rbx
 7fb:	41 89 fd             	mov    %edi,%r13d
 7fe:	49 89 f6             	mov    %rsi,%r14
 801:	4c 29 e5             	sub    %r12,%rbp
 804:	48 83 ec 08          	sub    $0x8,%rsp
 808:	48 c1 fd 03          	sar    $0x3,%rbp
 80c:	e8 af fd ff ff       	callq  5c0 <_init>
 811:	48 85 ed             	test   %rbp,%rbp
 814:	74 20                	je     836 <__libc_csu_init+0x56>
 816:	31 db                	xor    %ebx,%ebx
 818:	0f 1f 84 00 00 00 00 	nopl   0x0(%rax,%rax,1)
 81f:	00 
 820:	4c 89 fa             	mov    %r15,%rdx
 823:	4c 89 f6             	mov    %r14,%rsi
 826:	44 89 ef             	mov    %r13d,%edi
 829:	41 ff 14 dc          	callq  *(%r12,%rbx,8)
 82d:	48 83 c3 01          	add    $0x1,%rbx
 831:	48 39 dd             	cmp    %rbx,%rbp
 834:	75 ea                	jne    820 <__libc_csu_init+0x40>
 836:	48 83 c4 08          	add    $0x8,%rsp
 83a:	5b                   	pop    %rbx
 83b:	5d                   	pop    %rbp
 83c:	41 5c                	pop    %r12
 83e:	41 5d                	pop    %r13
 840:	41 5e                	pop    %r14
 842:	41 5f                	pop    %r15
 844:	c3                   	retq   
 845:	90                   	nop
 846:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 84d:	00 00 00 

0000000000000850 <__libc_csu_fini>:
 850:	f3 c3                	repz retq 

Disassembly of section .fini:

0000000000000854 <_fini>:
 854:	48 83 ec 08          	sub    $0x8,%rsp
 858:	48 83 c4 08          	add    $0x8,%rsp
 85c:	c3                   	retq   
