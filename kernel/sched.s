
sched.o:     file format elf32-i386


Disassembly of section .text:

00000000 <memset>:
   0:	55                   	push   %ebp
   1:	89 e5                	mov    %esp,%ebp
   3:	83 ec 14             	sub    $0x14,%esp
   6:	e8 fc ff ff ff       	call   7 <memset+0x7>
   b:	05 01 00 00 00       	add    $0x1,%eax
  10:	8b 45 0c             	mov    0xc(%ebp),%eax
  13:	88 45 ec             	mov    %al,-0x14(%ebp)
  16:	8b 45 08             	mov    0x8(%ebp),%eax
  19:	89 45 fc             	mov    %eax,-0x4(%ebp)
  1c:	eb 13                	jmp    31 <memset+0x31>
  1e:	8b 45 fc             	mov    -0x4(%ebp),%eax
  21:	8d 50 01             	lea    0x1(%eax),%edx
  24:	89 55 fc             	mov    %edx,-0x4(%ebp)
  27:	0f b6 55 ec          	movzbl -0x14(%ebp),%edx
  2b:	88 10                	mov    %dl,(%eax)
  2d:	83 6d 10 01          	subl   $0x1,0x10(%ebp)
  31:	83 7d 10 00          	cmpl   $0x0,0x10(%ebp)
  35:	75 e7                	jne    1e <memset+0x1e>
  37:	90                   	nop
  38:	c9                   	leave  
  39:	c3                   	ret    

0000003a <bzero>:
  3a:	55                   	push   %ebp
  3b:	89 e5                	mov    %esp,%ebp
  3d:	e8 fc ff ff ff       	call   3e <bzero+0x4>
  42:	05 01 00 00 00       	add    $0x1,%eax
  47:	ff 75 0c             	pushl  0xc(%ebp)
  4a:	6a 00                	push   $0x0
  4c:	ff 75 08             	pushl  0x8(%ebp)
  4f:	e8 ac ff ff ff       	call   0 <memset>
  54:	83 c4 0c             	add    $0xc,%esp
  57:	90                   	nop
  58:	c9                   	leave  
  59:	c3                   	ret    

0000005a <set_gdt>:
  5a:	55                   	push   %ebp
  5b:	89 e5                	mov    %esp,%ebp
  5d:	56                   	push   %esi
  5e:	53                   	push   %ebx
  5f:	83 ec 08             	sub    $0x8,%esp
  62:	e8 fc ff ff ff       	call   63 <set_gdt+0x9>
  67:	05 01 00 00 00       	add    $0x1,%eax
  6c:	8b 4d 14             	mov    0x14(%ebp),%ecx
  6f:	8b 55 18             	mov    0x18(%ebp),%edx
  72:	88 4d f4             	mov    %cl,-0xc(%ebp)
  75:	88 55 f0             	mov    %dl,-0x10(%ebp)
  78:	8b 55 10             	mov    0x10(%ebp),%edx
  7b:	89 d3                	mov    %edx,%ebx
  7d:	8b 90 00 00 00 00    	mov    0x0(%eax),%edx
  83:	8b 4d 08             	mov    0x8(%ebp),%ecx
  86:	66 89 1c ca          	mov    %bx,(%edx,%ecx,8)
  8a:	8b 55 0c             	mov    0xc(%ebp),%edx
  8d:	89 d3                	mov    %edx,%ebx
  8f:	8b 90 00 00 00 00    	mov    0x0(%eax),%edx
  95:	8b 4d 08             	mov    0x8(%ebp),%ecx
  98:	66 89 5c ca 02       	mov    %bx,0x2(%edx,%ecx,8)
  9d:	8b 55 0c             	mov    0xc(%ebp),%edx
  a0:	c1 ea 10             	shr    $0x10,%edx
  a3:	89 d3                	mov    %edx,%ebx
  a5:	8b 90 00 00 00 00    	mov    0x0(%eax),%edx
  ab:	8b 4d 08             	mov    0x8(%ebp),%ecx
  ae:	88 5c ca 04          	mov    %bl,0x4(%edx,%ecx,8)
  b2:	8b 90 00 00 00 00    	mov    0x0(%eax),%edx
  b8:	8b 4d 08             	mov    0x8(%ebp),%ecx
  bb:	0f b6 5d f4          	movzbl -0xc(%ebp),%ebx
  bf:	88 5c ca 05          	mov    %bl,0x5(%edx,%ecx,8)
  c3:	8b 55 10             	mov    0x10(%ebp),%edx
  c6:	c1 ea 10             	shr    $0x10,%edx
  c9:	83 e2 0f             	and    $0xf,%edx
  cc:	8b 88 00 00 00 00    	mov    0x0(%eax),%ecx
  d2:	8b 5d 08             	mov    0x8(%ebp),%ebx
  d5:	89 d6                	mov    %edx,%esi
  d7:	83 e6 0f             	and    $0xf,%esi
  da:	0f b6 54 d9 06       	movzbl 0x6(%ecx,%ebx,8),%edx
  df:	83 e2 f0             	and    $0xfffffff0,%edx
  e2:	09 f2                	or     %esi,%edx
  e4:	88 54 d9 06          	mov    %dl,0x6(%ecx,%ebx,8)
  e8:	0f b6 55 f0          	movzbl -0x10(%ebp),%edx
  ec:	89 d3                	mov    %edx,%ebx
  ee:	83 e3 0f             	and    $0xf,%ebx
  f1:	8b 90 00 00 00 00    	mov    0x0(%eax),%edx
  f7:	8b 4d 08             	mov    0x8(%ebp),%ecx
  fa:	89 de                	mov    %ebx,%esi
  fc:	c1 e6 04             	shl    $0x4,%esi
  ff:	0f b6 5c ca 06       	movzbl 0x6(%edx,%ecx,8),%ebx
 104:	83 e3 0f             	and    $0xf,%ebx
 107:	09 f3                	or     %esi,%ebx
 109:	88 5c ca 06          	mov    %bl,0x6(%edx,%ecx,8)
 10d:	8b 55 0c             	mov    0xc(%ebp),%edx
 110:	c1 ea 18             	shr    $0x18,%edx
 113:	89 d1                	mov    %edx,%ecx
 115:	8b 80 00 00 00 00    	mov    0x0(%eax),%eax
 11b:	8b 55 08             	mov    0x8(%ebp),%edx
 11e:	88 4c d0 07          	mov    %cl,0x7(%eax,%edx,8)
 122:	90                   	nop
 123:	83 c4 08             	add    $0x8,%esp
 126:	5b                   	pop    %ebx
 127:	5e                   	pop    %esi
 128:	5d                   	pop    %ebp
 129:	c3                   	ret    

0000012a <set_idt>:
 12a:	55                   	push   %ebp
 12b:	89 e5                	mov    %esp,%ebp
 12d:	53                   	push   %ebx
 12e:	83 ec 08             	sub    $0x8,%esp
 131:	e8 fc ff ff ff       	call   132 <set_idt+0x8>
 136:	05 01 00 00 00       	add    $0x1,%eax
 13b:	8b 4d 10             	mov    0x10(%ebp),%ecx
 13e:	8b 55 14             	mov    0x14(%ebp),%edx
 141:	66 89 4d f8          	mov    %cx,-0x8(%ebp)
 145:	66 89 55 f4          	mov    %dx,-0xc(%ebp)
 149:	8b 55 0c             	mov    0xc(%ebp),%edx
 14c:	89 d3                	mov    %edx,%ebx
 14e:	8b 90 00 00 00 00    	mov    0x0(%eax),%edx
 154:	8b 4d 08             	mov    0x8(%ebp),%ecx
 157:	66 89 1c ca          	mov    %bx,(%edx,%ecx,8)
 15b:	8b 55 0c             	mov    0xc(%ebp),%edx
 15e:	c1 ea 10             	shr    $0x10,%edx
 161:	89 d3                	mov    %edx,%ebx
 163:	8b 90 00 00 00 00    	mov    0x0(%eax),%edx
 169:	8b 4d 08             	mov    0x8(%ebp),%ecx
 16c:	66 89 5c ca 06       	mov    %bx,0x6(%edx,%ecx,8)
 171:	8b 90 00 00 00 00    	mov    0x0(%eax),%edx
 177:	8b 4d 08             	mov    0x8(%ebp),%ecx
 17a:	0f b7 5d f8          	movzwl -0x8(%ebp),%ebx
 17e:	66 89 5c ca 02       	mov    %bx,0x2(%edx,%ecx,8)
 183:	8b 90 00 00 00 00    	mov    0x0(%eax),%edx
 189:	8b 4d 08             	mov    0x8(%ebp),%ecx
 18c:	c6 44 ca 04 00       	movb   $0x0,0x4(%edx,%ecx,8)
 191:	0f b7 55 f4          	movzwl -0xc(%ebp),%edx
 195:	89 d1                	mov    %edx,%ecx
 197:	8b 80 00 00 00 00    	mov    0x0(%eax),%eax
 19d:	8b 55 08             	mov    0x8(%ebp),%edx
 1a0:	88 4c d0 05          	mov    %cl,0x5(%eax,%edx,8)
 1a4:	90                   	nop
 1a5:	83 c4 08             	add    $0x8,%esp
 1a8:	5b                   	pop    %ebx
 1a9:	5d                   	pop    %ebp
 1aa:	c3                   	ret    

000001ab <init_gdt>:
 1ab:	55                   	push   %ebp
 1ac:	89 e5                	mov    %esp,%ebp
 1ae:	53                   	push   %ebx
 1af:	83 ec 14             	sub    $0x14,%esp
 1b2:	e8 fc ff ff ff       	call   1b3 <init_gdt+0x8>
 1b7:	81 c3 02 00 00 00    	add    $0x2,%ebx
 1bd:	c7 45 f4 00 00 00 00 	movl   $0x0,-0xc(%ebp)
 1c4:	83 ec 0c             	sub    $0xc,%esp
 1c7:	8d 83 00 00 00 00    	lea    0x0(%ebx),%eax
 1cd:	50                   	push   %eax
 1ce:	e8 fc ff ff ff       	call   1cf <init_gdt+0x24>
 1d3:	83 c4 10             	add    $0x10,%esp
 1d6:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 1dc:	66 c7 00 ff 07       	movw   $0x7ff,(%eax)
 1e1:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 1e7:	89 c2                	mov    %eax,%edx
 1e9:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 1ef:	89 50 02             	mov    %edx,0x2(%eax)
 1f2:	83 ec 0c             	sub    $0xc,%esp
 1f5:	6a 00                	push   $0x0
 1f7:	6a 00                	push   $0x0
 1f9:	6a 00                	push   $0x0
 1fb:	6a 00                	push   $0x0
 1fd:	6a 00                	push   $0x0
 1ff:	e8 56 fe ff ff       	call   5a <set_gdt>
 204:	83 c4 20             	add    $0x20,%esp
 207:	83 ec 0c             	sub    $0xc,%esp
 20a:	6a 0c                	push   $0xc
 20c:	68 9a 00 00 00       	push   $0x9a
 211:	68 ff ff 0f 00       	push   $0xfffff
 216:	6a 00                	push   $0x0
 218:	6a 01                	push   $0x1
 21a:	e8 3b fe ff ff       	call   5a <set_gdt>
 21f:	83 c4 20             	add    $0x20,%esp
 222:	83 ec 0c             	sub    $0xc,%esp
 225:	6a 0c                	push   $0xc
 227:	68 92 00 00 00       	push   $0x92
 22c:	68 ff ff 0f 00       	push   $0xfffff
 231:	6a 00                	push   $0x0
 233:	6a 02                	push   $0x2
 235:	e8 20 fe ff ff       	call   5a <set_gdt>
 23a:	83 c4 20             	add    $0x20,%esp
 23d:	83 ec 0c             	sub    $0xc,%esp
 240:	6a 00                	push   $0x0
 242:	6a 00                	push   $0x0
 244:	6a 00                	push   $0x0
 246:	6a 00                	push   $0x0
 248:	6a 03                	push   $0x3
 24a:	e8 0b fe ff ff       	call   5a <set_gdt>
 24f:	83 c4 20             	add    $0x20,%esp
 252:	83 ec 0c             	sub    $0xc,%esp
 255:	6a 0c                	push   $0xc
 257:	68 fa 00 00 00       	push   $0xfa
 25c:	68 ff ff 0f 00       	push   $0xfffff
 261:	6a 00                	push   $0x0
 263:	6a 04                	push   $0x4
 265:	e8 f0 fd ff ff       	call   5a <set_gdt>
 26a:	83 c4 20             	add    $0x20,%esp
 26d:	83 ec 0c             	sub    $0xc,%esp
 270:	6a 0c                	push   $0xc
 272:	68 f2 00 00 00       	push   $0xf2
 277:	68 ff ff 0f 00       	push   $0xfffff
 27c:	6a 00                	push   $0x0
 27e:	6a 05                	push   $0x5
 280:	e8 d5 fd ff ff       	call   5a <set_gdt>
 285:	83 c4 20             	add    $0x20,%esp
 288:	c7 45 f4 06 00 00 00 	movl   $0x6,-0xc(%ebp)
 28f:	eb 1a                	jmp    2ab <init_gdt+0x100>
 291:	83 ec 0c             	sub    $0xc,%esp
 294:	6a 00                	push   $0x0
 296:	6a 00                	push   $0x0
 298:	6a 00                	push   $0x0
 29a:	6a 00                	push   $0x0
 29c:	ff 75 f4             	pushl  -0xc(%ebp)
 29f:	e8 b6 fd ff ff       	call   5a <set_gdt>
 2a4:	83 c4 20             	add    $0x20,%esp
 2a7:	83 45 f4 01          	addl   $0x1,-0xc(%ebp)
 2ab:	81 7d f4 ff 00 00 00 	cmpl   $0xff,-0xc(%ebp)
 2b2:	7e dd                	jle    291 <init_gdt+0xe6>
 2b4:	83 ec 0c             	sub    $0xc,%esp
 2b7:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 2bd:	50                   	push   %eax
 2be:	e8 fc ff ff ff       	call   2bf <init_gdt+0x114>
 2c3:	83 c4 10             	add    $0x10,%esp
 2c6:	90                   	nop
 2c7:	8b 5d fc             	mov    -0x4(%ebp),%ebx
 2ca:	c9                   	leave  
 2cb:	c3                   	ret    

000002cc <init_idt>:
 2cc:	55                   	push   %ebp
 2cd:	89 e5                	mov    %esp,%ebp
 2cf:	53                   	push   %ebx
 2d0:	83 ec 04             	sub    $0x4,%esp
 2d3:	e8 fc ff ff ff       	call   2d4 <init_idt+0x8>
 2d8:	81 c3 02 00 00 00    	add    $0x2,%ebx
 2de:	83 ec 0c             	sub    $0xc,%esp
 2e1:	8d 83 13 00 00 00    	lea    0x13(%ebx),%eax
 2e7:	50                   	push   %eax
 2e8:	e8 fc ff ff ff       	call   2e9 <init_idt+0x1d>
 2ed:	83 c4 10             	add    $0x10,%esp
 2f0:	83 ec 08             	sub    $0x8,%esp
 2f3:	6a 11                	push   $0x11
 2f5:	6a 20                	push   $0x20
 2f7:	e8 fc ff ff ff       	call   2f8 <init_idt+0x2c>
 2fc:	83 c4 10             	add    $0x10,%esp
 2ff:	83 ec 08             	sub    $0x8,%esp
 302:	6a 11                	push   $0x11
 304:	68 a0 00 00 00       	push   $0xa0
 309:	e8 fc ff ff ff       	call   30a <init_idt+0x3e>
 30e:	83 c4 10             	add    $0x10,%esp
 311:	83 ec 08             	sub    $0x8,%esp
 314:	6a 20                	push   $0x20
 316:	6a 21                	push   $0x21
 318:	e8 fc ff ff ff       	call   319 <init_idt+0x4d>
 31d:	83 c4 10             	add    $0x10,%esp
 320:	83 ec 08             	sub    $0x8,%esp
 323:	6a 28                	push   $0x28
 325:	68 a1 00 00 00       	push   $0xa1
 32a:	e8 fc ff ff ff       	call   32b <init_idt+0x5f>
 32f:	83 c4 10             	add    $0x10,%esp
 332:	83 ec 08             	sub    $0x8,%esp
 335:	6a 04                	push   $0x4
 337:	6a 21                	push   $0x21
 339:	e8 fc ff ff ff       	call   33a <init_idt+0x6e>
 33e:	83 c4 10             	add    $0x10,%esp
 341:	83 ec 08             	sub    $0x8,%esp
 344:	6a 02                	push   $0x2
 346:	68 a1 00 00 00       	push   $0xa1
 34b:	e8 fc ff ff ff       	call   34c <init_idt+0x80>
 350:	83 c4 10             	add    $0x10,%esp
 353:	83 ec 08             	sub    $0x8,%esp
 356:	6a 01                	push   $0x1
 358:	6a 21                	push   $0x21
 35a:	e8 fc ff ff ff       	call   35b <init_idt+0x8f>
 35f:	83 c4 10             	add    $0x10,%esp
 362:	83 ec 08             	sub    $0x8,%esp
 365:	6a 01                	push   $0x1
 367:	68 a1 00 00 00       	push   $0xa1
 36c:	e8 fc ff ff ff       	call   36d <init_idt+0xa1>
 371:	83 c4 10             	add    $0x10,%esp
 374:	83 ec 08             	sub    $0x8,%esp
 377:	6a 00                	push   $0x0
 379:	6a 21                	push   $0x21
 37b:	e8 fc ff ff ff       	call   37c <init_idt+0xb0>
 380:	83 c4 10             	add    $0x10,%esp
 383:	83 ec 08             	sub    $0x8,%esp
 386:	6a 00                	push   $0x0
 388:	68 a1 00 00 00       	push   $0xa1
 38d:	e8 fc ff ff ff       	call   38e <init_idt+0xc2>
 392:	83 c4 10             	add    $0x10,%esp
 395:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 39b:	66 c7 00 ff 07       	movw   $0x7ff,(%eax)
 3a0:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 3a6:	89 c2                	mov    %eax,%edx
 3a8:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 3ae:	89 50 02             	mov    %edx,0x2(%eax)
 3b1:	83 ec 08             	sub    $0x8,%esp
 3b4:	68 00 04 00 00       	push   $0x400
 3b9:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 3bf:	50                   	push   %eax
 3c0:	e8 75 fc ff ff       	call   3a <bzero>
 3c5:	83 c4 10             	add    $0x10,%esp
 3c8:	83 ec 08             	sub    $0x8,%esp
 3cb:	68 00 08 00 00       	push   $0x800
 3d0:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 3d6:	50                   	push   %eax
 3d7:	e8 5e fc ff ff       	call   3a <bzero>
 3dc:	83 c4 10             	add    $0x10,%esp
 3df:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 3e5:	68 8e 00 00 00       	push   $0x8e
 3ea:	6a 08                	push   $0x8
 3ec:	50                   	push   %eax
 3ed:	6a 00                	push   $0x0
 3ef:	e8 36 fd ff ff       	call   12a <set_idt>
 3f4:	83 c4 10             	add    $0x10,%esp
 3f7:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 3fd:	68 8e 00 00 00       	push   $0x8e
 402:	6a 08                	push   $0x8
 404:	50                   	push   %eax
 405:	6a 01                	push   $0x1
 407:	e8 1e fd ff ff       	call   12a <set_idt>
 40c:	83 c4 10             	add    $0x10,%esp
 40f:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 415:	68 8e 00 00 00       	push   $0x8e
 41a:	6a 08                	push   $0x8
 41c:	50                   	push   %eax
 41d:	6a 02                	push   $0x2
 41f:	e8 06 fd ff ff       	call   12a <set_idt>
 424:	83 c4 10             	add    $0x10,%esp
 427:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 42d:	68 8e 00 00 00       	push   $0x8e
 432:	6a 08                	push   $0x8
 434:	50                   	push   %eax
 435:	6a 03                	push   $0x3
 437:	e8 ee fc ff ff       	call   12a <set_idt>
 43c:	83 c4 10             	add    $0x10,%esp
 43f:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 445:	68 8e 00 00 00       	push   $0x8e
 44a:	6a 08                	push   $0x8
 44c:	50                   	push   %eax
 44d:	6a 04                	push   $0x4
 44f:	e8 d6 fc ff ff       	call   12a <set_idt>
 454:	83 c4 10             	add    $0x10,%esp
 457:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 45d:	68 8e 00 00 00       	push   $0x8e
 462:	6a 08                	push   $0x8
 464:	50                   	push   %eax
 465:	6a 05                	push   $0x5
 467:	e8 be fc ff ff       	call   12a <set_idt>
 46c:	83 c4 10             	add    $0x10,%esp
 46f:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 475:	68 8e 00 00 00       	push   $0x8e
 47a:	6a 08                	push   $0x8
 47c:	50                   	push   %eax
 47d:	6a 06                	push   $0x6
 47f:	e8 a6 fc ff ff       	call   12a <set_idt>
 484:	83 c4 10             	add    $0x10,%esp
 487:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 48d:	68 8e 00 00 00       	push   $0x8e
 492:	6a 08                	push   $0x8
 494:	50                   	push   %eax
 495:	6a 07                	push   $0x7
 497:	e8 8e fc ff ff       	call   12a <set_idt>
 49c:	83 c4 10             	add    $0x10,%esp
 49f:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 4a5:	68 8e 00 00 00       	push   $0x8e
 4aa:	6a 08                	push   $0x8
 4ac:	50                   	push   %eax
 4ad:	6a 08                	push   $0x8
 4af:	e8 76 fc ff ff       	call   12a <set_idt>
 4b4:	83 c4 10             	add    $0x10,%esp
 4b7:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 4bd:	68 8e 00 00 00       	push   $0x8e
 4c2:	6a 08                	push   $0x8
 4c4:	50                   	push   %eax
 4c5:	6a 09                	push   $0x9
 4c7:	e8 5e fc ff ff       	call   12a <set_idt>
 4cc:	83 c4 10             	add    $0x10,%esp
 4cf:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 4d5:	68 8e 00 00 00       	push   $0x8e
 4da:	6a 08                	push   $0x8
 4dc:	50                   	push   %eax
 4dd:	6a 0a                	push   $0xa
 4df:	e8 46 fc ff ff       	call   12a <set_idt>
 4e4:	83 c4 10             	add    $0x10,%esp
 4e7:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 4ed:	68 8e 00 00 00       	push   $0x8e
 4f2:	6a 08                	push   $0x8
 4f4:	50                   	push   %eax
 4f5:	6a 0b                	push   $0xb
 4f7:	e8 2e fc ff ff       	call   12a <set_idt>
 4fc:	83 c4 10             	add    $0x10,%esp
 4ff:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 505:	68 8e 00 00 00       	push   $0x8e
 50a:	6a 08                	push   $0x8
 50c:	50                   	push   %eax
 50d:	6a 0c                	push   $0xc
 50f:	e8 16 fc ff ff       	call   12a <set_idt>
 514:	83 c4 10             	add    $0x10,%esp
 517:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 51d:	68 8e 00 00 00       	push   $0x8e
 522:	6a 08                	push   $0x8
 524:	50                   	push   %eax
 525:	6a 0d                	push   $0xd
 527:	e8 fe fb ff ff       	call   12a <set_idt>
 52c:	83 c4 10             	add    $0x10,%esp
 52f:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 535:	68 8e 00 00 00       	push   $0x8e
 53a:	6a 08                	push   $0x8
 53c:	50                   	push   %eax
 53d:	6a 0e                	push   $0xe
 53f:	e8 e6 fb ff ff       	call   12a <set_idt>
 544:	83 c4 10             	add    $0x10,%esp
 547:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 54d:	68 8e 00 00 00       	push   $0x8e
 552:	6a 08                	push   $0x8
 554:	50                   	push   %eax
 555:	6a 0f                	push   $0xf
 557:	e8 ce fb ff ff       	call   12a <set_idt>
 55c:	83 c4 10             	add    $0x10,%esp
 55f:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 565:	68 8e 00 00 00       	push   $0x8e
 56a:	6a 08                	push   $0x8
 56c:	50                   	push   %eax
 56d:	6a 10                	push   $0x10
 56f:	e8 b6 fb ff ff       	call   12a <set_idt>
 574:	83 c4 10             	add    $0x10,%esp
 577:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 57d:	68 8e 00 00 00       	push   $0x8e
 582:	6a 08                	push   $0x8
 584:	50                   	push   %eax
 585:	6a 11                	push   $0x11
 587:	e8 9e fb ff ff       	call   12a <set_idt>
 58c:	83 c4 10             	add    $0x10,%esp
 58f:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 595:	68 8e 00 00 00       	push   $0x8e
 59a:	6a 08                	push   $0x8
 59c:	50                   	push   %eax
 59d:	6a 12                	push   $0x12
 59f:	e8 86 fb ff ff       	call   12a <set_idt>
 5a4:	83 c4 10             	add    $0x10,%esp
 5a7:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 5ad:	68 8e 00 00 00       	push   $0x8e
 5b2:	6a 08                	push   $0x8
 5b4:	50                   	push   %eax
 5b5:	6a 13                	push   $0x13
 5b7:	e8 6e fb ff ff       	call   12a <set_idt>
 5bc:	83 c4 10             	add    $0x10,%esp
 5bf:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 5c5:	68 8e 00 00 00       	push   $0x8e
 5ca:	6a 08                	push   $0x8
 5cc:	50                   	push   %eax
 5cd:	6a 14                	push   $0x14
 5cf:	e8 56 fb ff ff       	call   12a <set_idt>
 5d4:	83 c4 10             	add    $0x10,%esp
 5d7:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 5dd:	68 8e 00 00 00       	push   $0x8e
 5e2:	6a 08                	push   $0x8
 5e4:	50                   	push   %eax
 5e5:	6a 15                	push   $0x15
 5e7:	e8 3e fb ff ff       	call   12a <set_idt>
 5ec:	83 c4 10             	add    $0x10,%esp
 5ef:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 5f5:	68 8e 00 00 00       	push   $0x8e
 5fa:	6a 08                	push   $0x8
 5fc:	50                   	push   %eax
 5fd:	6a 16                	push   $0x16
 5ff:	e8 26 fb ff ff       	call   12a <set_idt>
 604:	83 c4 10             	add    $0x10,%esp
 607:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 60d:	68 8e 00 00 00       	push   $0x8e
 612:	6a 08                	push   $0x8
 614:	50                   	push   %eax
 615:	6a 17                	push   $0x17
 617:	e8 0e fb ff ff       	call   12a <set_idt>
 61c:	83 c4 10             	add    $0x10,%esp
 61f:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 625:	68 8e 00 00 00       	push   $0x8e
 62a:	6a 08                	push   $0x8
 62c:	50                   	push   %eax
 62d:	6a 18                	push   $0x18
 62f:	e8 f6 fa ff ff       	call   12a <set_idt>
 634:	83 c4 10             	add    $0x10,%esp
 637:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 63d:	68 8e 00 00 00       	push   $0x8e
 642:	6a 08                	push   $0x8
 644:	50                   	push   %eax
 645:	6a 19                	push   $0x19
 647:	e8 de fa ff ff       	call   12a <set_idt>
 64c:	83 c4 10             	add    $0x10,%esp
 64f:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 655:	68 8e 00 00 00       	push   $0x8e
 65a:	6a 08                	push   $0x8
 65c:	50                   	push   %eax
 65d:	6a 1a                	push   $0x1a
 65f:	e8 c6 fa ff ff       	call   12a <set_idt>
 664:	83 c4 10             	add    $0x10,%esp
 667:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 66d:	68 8e 00 00 00       	push   $0x8e
 672:	6a 08                	push   $0x8
 674:	50                   	push   %eax
 675:	6a 1b                	push   $0x1b
 677:	e8 ae fa ff ff       	call   12a <set_idt>
 67c:	83 c4 10             	add    $0x10,%esp
 67f:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 685:	68 8e 00 00 00       	push   $0x8e
 68a:	6a 08                	push   $0x8
 68c:	50                   	push   %eax
 68d:	6a 1c                	push   $0x1c
 68f:	e8 96 fa ff ff       	call   12a <set_idt>
 694:	83 c4 10             	add    $0x10,%esp
 697:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 69d:	68 8e 00 00 00       	push   $0x8e
 6a2:	6a 08                	push   $0x8
 6a4:	50                   	push   %eax
 6a5:	6a 1d                	push   $0x1d
 6a7:	e8 7e fa ff ff       	call   12a <set_idt>
 6ac:	83 c4 10             	add    $0x10,%esp
 6af:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 6b5:	68 8e 00 00 00       	push   $0x8e
 6ba:	6a 08                	push   $0x8
 6bc:	50                   	push   %eax
 6bd:	6a 1e                	push   $0x1e
 6bf:	e8 66 fa ff ff       	call   12a <set_idt>
 6c4:	83 c4 10             	add    $0x10,%esp
 6c7:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 6cd:	68 8e 00 00 00       	push   $0x8e
 6d2:	6a 08                	push   $0x8
 6d4:	50                   	push   %eax
 6d5:	6a 1f                	push   $0x1f
 6d7:	e8 4e fa ff ff       	call   12a <set_idt>
 6dc:	83 c4 10             	add    $0x10,%esp
 6df:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 6e5:	68 8e 00 00 00       	push   $0x8e
 6ea:	6a 08                	push   $0x8
 6ec:	50                   	push   %eax
 6ed:	6a 20                	push   $0x20
 6ef:	e8 36 fa ff ff       	call   12a <set_idt>
 6f4:	83 c4 10             	add    $0x10,%esp
 6f7:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 6fd:	68 8e 00 00 00       	push   $0x8e
 702:	6a 08                	push   $0x8
 704:	50                   	push   %eax
 705:	6a 21                	push   $0x21
 707:	e8 1e fa ff ff       	call   12a <set_idt>
 70c:	83 c4 10             	add    $0x10,%esp
 70f:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 715:	68 8e 00 00 00       	push   $0x8e
 71a:	6a 08                	push   $0x8
 71c:	50                   	push   %eax
 71d:	6a 22                	push   $0x22
 71f:	e8 06 fa ff ff       	call   12a <set_idt>
 724:	83 c4 10             	add    $0x10,%esp
 727:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 72d:	68 8e 00 00 00       	push   $0x8e
 732:	6a 08                	push   $0x8
 734:	50                   	push   %eax
 735:	6a 23                	push   $0x23
 737:	e8 ee f9 ff ff       	call   12a <set_idt>
 73c:	83 c4 10             	add    $0x10,%esp
 73f:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 745:	68 8e 00 00 00       	push   $0x8e
 74a:	6a 08                	push   $0x8
 74c:	50                   	push   %eax
 74d:	6a 24                	push   $0x24
 74f:	e8 d6 f9 ff ff       	call   12a <set_idt>
 754:	83 c4 10             	add    $0x10,%esp
 757:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 75d:	68 8e 00 00 00       	push   $0x8e
 762:	6a 08                	push   $0x8
 764:	50                   	push   %eax
 765:	6a 25                	push   $0x25
 767:	e8 be f9 ff ff       	call   12a <set_idt>
 76c:	83 c4 10             	add    $0x10,%esp
 76f:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 775:	68 8e 00 00 00       	push   $0x8e
 77a:	6a 08                	push   $0x8
 77c:	50                   	push   %eax
 77d:	6a 26                	push   $0x26
 77f:	e8 a6 f9 ff ff       	call   12a <set_idt>
 784:	83 c4 10             	add    $0x10,%esp
 787:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 78d:	68 8e 00 00 00       	push   $0x8e
 792:	6a 08                	push   $0x8
 794:	50                   	push   %eax
 795:	6a 27                	push   $0x27
 797:	e8 8e f9 ff ff       	call   12a <set_idt>
 79c:	83 c4 10             	add    $0x10,%esp
 79f:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 7a5:	68 8e 00 00 00       	push   $0x8e
 7aa:	6a 08                	push   $0x8
 7ac:	50                   	push   %eax
 7ad:	6a 28                	push   $0x28
 7af:	e8 76 f9 ff ff       	call   12a <set_idt>
 7b4:	83 c4 10             	add    $0x10,%esp
 7b7:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 7bd:	68 8e 00 00 00       	push   $0x8e
 7c2:	6a 08                	push   $0x8
 7c4:	50                   	push   %eax
 7c5:	6a 29                	push   $0x29
 7c7:	e8 5e f9 ff ff       	call   12a <set_idt>
 7cc:	83 c4 10             	add    $0x10,%esp
 7cf:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 7d5:	68 8e 00 00 00       	push   $0x8e
 7da:	6a 08                	push   $0x8
 7dc:	50                   	push   %eax
 7dd:	6a 2a                	push   $0x2a
 7df:	e8 46 f9 ff ff       	call   12a <set_idt>
 7e4:	83 c4 10             	add    $0x10,%esp
 7e7:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 7ed:	68 8e 00 00 00       	push   $0x8e
 7f2:	6a 08                	push   $0x8
 7f4:	50                   	push   %eax
 7f5:	6a 2b                	push   $0x2b
 7f7:	e8 2e f9 ff ff       	call   12a <set_idt>
 7fc:	83 c4 10             	add    $0x10,%esp
 7ff:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 805:	68 8e 00 00 00       	push   $0x8e
 80a:	6a 08                	push   $0x8
 80c:	50                   	push   %eax
 80d:	6a 2c                	push   $0x2c
 80f:	e8 16 f9 ff ff       	call   12a <set_idt>
 814:	83 c4 10             	add    $0x10,%esp
 817:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 81d:	68 8e 00 00 00       	push   $0x8e
 822:	6a 08                	push   $0x8
 824:	50                   	push   %eax
 825:	6a 2d                	push   $0x2d
 827:	e8 fe f8 ff ff       	call   12a <set_idt>
 82c:	83 c4 10             	add    $0x10,%esp
 82f:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 835:	68 8e 00 00 00       	push   $0x8e
 83a:	6a 08                	push   $0x8
 83c:	50                   	push   %eax
 83d:	6a 2e                	push   $0x2e
 83f:	e8 e6 f8 ff ff       	call   12a <set_idt>
 844:	83 c4 10             	add    $0x10,%esp
 847:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 84d:	68 8e 00 00 00       	push   $0x8e
 852:	6a 08                	push   $0x8
 854:	50                   	push   %eax
 855:	6a 2f                	push   $0x2f
 857:	e8 ce f8 ff ff       	call   12a <set_idt>
 85c:	83 c4 10             	add    $0x10,%esp
 85f:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 865:	68 8e 00 00 00       	push   $0x8e
 86a:	6a 08                	push   $0x8
 86c:	50                   	push   %eax
 86d:	68 ff 00 00 00       	push   $0xff
 872:	e8 b3 f8 ff ff       	call   12a <set_idt>
 877:	83 c4 10             	add    $0x10,%esp
 87a:	83 ec 0c             	sub    $0xc,%esp
 87d:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
 883:	50                   	push   %eax
 884:	e8 fc ff ff ff       	call   885 <init_idt+0x5b9>
 889:	83 c4 10             	add    $0x10,%esp
 88c:	90                   	nop
 88d:	8b 5d fc             	mov    -0x4(%ebp),%ebx
 890:	c9                   	leave  
 891:	c3                   	ret    

00000892 <init0>:
 892:	55                   	push   %ebp
 893:	89 e5                	mov    %esp,%ebp
 895:	83 ec 10             	sub    $0x10,%esp
 898:	e8 fc ff ff ff       	call   899 <init0+0x7>
 89d:	05 01 00 00 00       	add    $0x1,%eax
 8a2:	c7 45 fc 00 00 00 00 	movl   $0x0,-0x4(%ebp)
 8a9:	83 45 fc 01          	addl   $0x1,-0x4(%ebp)
 8ad:	eb fe                	jmp    8ad <init0+0x1b>

000008af <schedule_init>:
 8af:	55                   	push   %ebp
 8b0:	89 e5                	mov    %esp,%ebp
 8b2:	57                   	push   %edi
 8b3:	56                   	push   %esi
 8b4:	53                   	push   %ebx
 8b5:	83 ec 1c             	sub    $0x1c,%esp
 8b8:	e8 fc ff ff ff       	call   8b9 <schedule_init+0xa>
 8bd:	05 01 00 00 00       	add    $0x1,%eax
 8c2:	89 45 e4             	mov    %eax,-0x1c(%ebp)
 8c5:	83 ec 0c             	sub    $0xc,%esp
 8c8:	8b 45 e4             	mov    -0x1c(%ebp),%eax
 8cb:	8d 80 24 00 00 00    	lea    0x24(%eax),%eax
 8d1:	50                   	push   %eax
 8d2:	8b 5d e4             	mov    -0x1c(%ebp),%ebx
 8d5:	e8 fc ff ff ff       	call   8d6 <schedule_init+0x27>
 8da:	83 c4 10             	add    $0x10,%esp
 8dd:	8b 45 e4             	mov    -0x1c(%ebp),%eax
 8e0:	8d 80 80 00 00 00    	lea    0x80(%eax),%eax
 8e6:	89 45 e0             	mov    %eax,-0x20(%ebp)
 8e9:	8b 45 e4             	mov    -0x1c(%ebp),%eax
 8ec:	8b 80 00 00 00 00    	mov    0x0(%eax),%eax
 8f2:	8d 40 20             	lea    0x20(%eax),%eax
 8f5:	89 c6                	mov    %eax,%esi
 8f7:	8b 45 e4             	mov    -0x1c(%ebp),%eax
 8fa:	8b 80 00 00 00 00    	mov    0x0(%eax),%eax
 900:	8d 40 22             	lea    0x22(%eax),%eax
 903:	89 c7                	mov    %eax,%edi
 905:	8b 45 e4             	mov    -0x1c(%ebp),%eax
 908:	8b 80 00 00 00 00    	mov    0x0(%eax),%eax
 90e:	8d 40 24             	lea    0x24(%eax),%eax
 911:	89 45 dc             	mov    %eax,-0x24(%ebp)
 914:	8b 45 e4             	mov    -0x1c(%ebp),%eax
 917:	8b 80 00 00 00 00    	mov    0x0(%eax),%eax
 91d:	8d 50 25             	lea    0x25(%eax),%edx
 920:	8b 45 e4             	mov    -0x1c(%ebp),%eax
 923:	8b 80 00 00 00 00    	mov    0x0(%eax),%eax
 929:	8d 48 26             	lea    0x26(%eax),%ecx
 92c:	8b 45 e4             	mov    -0x1c(%ebp),%eax
 92f:	8b 80 00 00 00 00    	mov    0x0(%eax),%eax
 935:	8d 40 27             	lea    0x27(%eax),%eax
 938:	89 c3                	mov    %eax,%ebx
 93a:	89 d0                	mov    %edx,%eax
 93c:	89 ca                	mov    %ecx,%edx
 93e:	89 f1                	mov    %esi,%ecx
 940:	89 fe                	mov    %edi,%esi
 942:	8b 7d dc             	mov    -0x24(%ebp),%edi
 945:	66 c7 01 68 00       	movw   $0x68,(%ecx)
 94a:	66 89 06             	mov    %ax,(%esi)
 94d:	c1 c8 10             	ror    $0x10,%eax
 950:	88 07                	mov    %al,(%edi)
 952:	c6 00 89             	movb   $0x89,(%eax)
 955:	c6 02 00             	movb   $0x0,(%edx)
 958:	88 23                	mov    %ah,(%ebx)
 95a:	c1 c8 10             	ror    $0x10,%eax
 95d:	8b 45 e4             	mov    -0x1c(%ebp),%eax
 960:	8d 80 68 00 00 00    	lea    0x68(%eax),%eax
 966:	89 45 e0             	mov    %eax,-0x20(%ebp)
 969:	8b 45 e4             	mov    -0x1c(%ebp),%eax
 96c:	8b 80 00 00 00 00    	mov    0x0(%eax),%eax
 972:	8d 40 28             	lea    0x28(%eax),%eax
 975:	89 c6                	mov    %eax,%esi
 977:	8b 45 e4             	mov    -0x1c(%ebp),%eax
 97a:	8b 80 00 00 00 00    	mov    0x0(%eax),%eax
 980:	8d 40 2a             	lea    0x2a(%eax),%eax
 983:	89 c7                	mov    %eax,%edi
 985:	8b 45 e4             	mov    -0x1c(%ebp),%eax
 988:	8b 80 00 00 00 00    	mov    0x0(%eax),%eax
 98e:	8d 40 2c             	lea    0x2c(%eax),%eax
 991:	89 45 dc             	mov    %eax,-0x24(%ebp)
 994:	8b 45 e4             	mov    -0x1c(%ebp),%eax
 997:	8b 80 00 00 00 00    	mov    0x0(%eax),%eax
 99d:	8d 50 2d             	lea    0x2d(%eax),%edx
 9a0:	8b 45 e4             	mov    -0x1c(%ebp),%eax
 9a3:	8b 80 00 00 00 00    	mov    0x0(%eax),%eax
 9a9:	8d 48 2e             	lea    0x2e(%eax),%ecx
 9ac:	8b 45 e4             	mov    -0x1c(%ebp),%eax
 9af:	8b 80 00 00 00 00    	mov    0x0(%eax),%eax
 9b5:	8d 40 2f             	lea    0x2f(%eax),%eax
 9b8:	89 c3                	mov    %eax,%ebx
 9ba:	89 d0                	mov    %edx,%eax
 9bc:	89 ca                	mov    %ecx,%edx
 9be:	89 f1                	mov    %esi,%ecx
 9c0:	89 fe                	mov    %edi,%esi
 9c2:	8b 7d dc             	mov    -0x24(%ebp),%edi
 9c5:	66 c7 01 68 00       	movw   $0x68,(%ecx)
 9ca:	66 89 06             	mov    %ax,(%esi)
 9cd:	c1 c8 10             	ror    $0x10,%eax
 9d0:	88 07                	mov    %al,(%edi)
 9d2:	c6 00 82             	movb   $0x82,(%eax)
 9d5:	c6 02 00             	movb   $0x0,(%edx)
 9d8:	88 23                	mov    %ah,(%ebx)
 9da:	c1 c8 10             	ror    $0x10,%eax
 9dd:	83 ec 0c             	sub    $0xc,%esp
 9e0:	8b 45 e4             	mov    -0x1c(%ebp),%eax
 9e3:	8d 80 38 00 00 00    	lea    0x38(%eax),%eax
 9e9:	50                   	push   %eax
 9ea:	8b 5d e4             	mov    -0x1c(%ebp),%ebx
 9ed:	e8 fc ff ff ff       	call   9ee <schedule_init+0x13f>
 9f2:	83 c4 10             	add    $0x10,%esp
 9f5:	b8 20 00 00 00       	mov    $0x20,%eax
 9fa:	0f 00 d8             	ltr    %ax
 9fd:	b8 28 00 00 00       	mov    $0x28,%eax
 a02:	0f 00 d0             	lldt   %ax
 a05:	83 ec 0c             	sub    $0xc,%esp
 a08:	6a 64                	push   $0x64
 a0a:	8b 5d e4             	mov    -0x1c(%ebp),%ebx
 a0d:	e8 fc ff ff ff       	call   a0e <schedule_init+0x15f>
 a12:	83 c4 10             	add    $0x10,%esp
 a15:	8b 45 e4             	mov    -0x1c(%ebp),%eax
 a18:	8b 80 00 00 00 00    	mov    0x0(%eax),%eax
 a1e:	68 8e 00 00 00       	push   $0x8e
 a23:	6a 08                	push   $0x8
 a25:	50                   	push   %eax
 a26:	68 80 00 00 00       	push   $0x80
 a2b:	e8 fa f6 ff ff       	call   12a <set_idt>
 a30:	83 c4 10             	add    $0x10,%esp
 a33:	83 ec 0c             	sub    $0xc,%esp
 a36:	8b 45 e4             	mov    -0x1c(%ebp),%eax
 a39:	8d 80 4b 00 00 00    	lea    0x4b(%eax),%eax
 a3f:	50                   	push   %eax
 a40:	8b 5d e4             	mov    -0x1c(%ebp),%ebx
 a43:	e8 fc ff ff ff       	call   a44 <schedule_init+0x195>
 a48:	83 c4 10             	add    $0x10,%esp
 a4b:	90                   	nop
 a4c:	8d 65 f4             	lea    -0xc(%ebp),%esp
 a4f:	5b                   	pop    %ebx
 a50:	5e                   	pop    %esi
 a51:	5f                   	pop    %edi
 a52:	5d                   	pop    %ebp
 a53:	c3                   	ret    

00000a54 <reschedule>:
 a54:	55                   	push   %ebp
 a55:	89 e5                	mov    %esp,%ebp
 a57:	e8 fc ff ff ff       	call   a58 <reschedule+0x4>
 a5c:	05 01 00 00 00       	add    $0x1,%eax
 a61:	90                   	nop
 a62:	5d                   	pop    %ebp
 a63:	c3                   	ret    

Disassembly of section .text.__x86.get_pc_thunk.ax:

00000000 <__x86.get_pc_thunk.ax>:
   0:	8b 04 24             	mov    (%esp),%eax
   3:	c3                   	ret    

Disassembly of section .text.__x86.get_pc_thunk.bx:

00000000 <__x86.get_pc_thunk.bx>:
   0:	8b 1c 24             	mov    (%esp),%ebx
   3:	c3                   	ret    
