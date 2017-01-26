
	.text

#define ISR_ENTRY(n) \
isr ## n: \
	pushq $0; \
	pushq $n; \
	jmp common

#define ISR_MENTRY(n) \
isr ## n: \
	pushq $n; \
	jmp common


ISR_ENTRY(0)
ISR_ENTRY(1)
ISR_ENTRY(2)
ISR_ENTRY(3)
ISR_ENTRY(4)
ISR_ENTRY(5)
ISR_ENTRY(6)
ISR_ENTRY(7)
ISR_MENTRY(8)
ISR_ENTRY(9)
ISR_MENTRY(10)
ISR_MENTRY(11)
ISR_MENTRY(12)
ISR_MENTRY(13)
ISR_MENTRY(14)
ISR_ENTRY(15)
ISR_ENTRY(16)
ISR_MENTRY(17)
ISR_ENTRY(18)
ISR_ENTRY(19)
ISR_ENTRY(20)
ISR_ENTRY(21)
ISR_ENTRY(22)
ISR_ENTRY(23)
ISR_ENTRY(24)
ISR_ENTRY(25)
ISR_ENTRY(26)
ISR_ENTRY(27)
ISR_ENTRY(28)
ISR_ENTRY(29)
ISR_ENTRY(30)
ISR_ENTRY(31)
ISR_ENTRY(32)
ISR_ENTRY(33)
ISR_ENTRY(34)
ISR_ENTRY(35)
ISR_ENTRY(36)
ISR_ENTRY(37)
ISR_ENTRY(38)
ISR_ENTRY(39)
ISR_ENTRY(40)
ISR_ENTRY(41)
ISR_ENTRY(42)
ISR_ENTRY(43)
ISR_ENTRY(44)
ISR_ENTRY(45)
ISR_ENTRY(46)
ISR_ENTRY(47)
ISR_ENTRY(48)
ISR_ENTRY(49)
ISR_ENTRY(50)
ISR_ENTRY(51)
ISR_ENTRY(52)
ISR_ENTRY(53)
ISR_ENTRY(54)
ISR_ENTRY(55)
ISR_ENTRY(56)
ISR_ENTRY(57)
ISR_ENTRY(58)
ISR_ENTRY(59)
ISR_ENTRY(60)
ISR_ENTRY(61)
ISR_ENTRY(62)
ISR_ENTRY(63)
ISR_ENTRY(64)
ISR_ENTRY(65)
ISR_ENTRY(66)
ISR_ENTRY(67)
ISR_ENTRY(68)
ISR_ENTRY(69)
ISR_ENTRY(70)
ISR_ENTRY(71)
ISR_ENTRY(72)
ISR_ENTRY(73)
ISR_ENTRY(74)
ISR_ENTRY(75)
ISR_ENTRY(76)
ISR_ENTRY(77)
ISR_ENTRY(78)
ISR_ENTRY(79)
ISR_ENTRY(80)
ISR_ENTRY(81)
ISR_ENTRY(82)
ISR_ENTRY(83)
ISR_ENTRY(84)
ISR_ENTRY(85)
ISR_ENTRY(86)
ISR_ENTRY(87)
ISR_ENTRY(88)
ISR_ENTRY(89)
ISR_ENTRY(90)
ISR_ENTRY(91)
ISR_ENTRY(92)
ISR_ENTRY(93)
ISR_ENTRY(94)
ISR_ENTRY(95)
ISR_ENTRY(96)
ISR_ENTRY(97)
ISR_ENTRY(98)
ISR_ENTRY(99)
ISR_ENTRY(100)
ISR_ENTRY(101)
ISR_ENTRY(102)
ISR_ENTRY(103)
ISR_ENTRY(104)
ISR_ENTRY(105)
ISR_ENTRY(106)
ISR_ENTRY(107)
ISR_ENTRY(108)
ISR_ENTRY(109)
ISR_ENTRY(110)
ISR_ENTRY(111)
ISR_ENTRY(112)
ISR_ENTRY(113)
ISR_ENTRY(114)
ISR_ENTRY(115)
ISR_ENTRY(116)
ISR_ENTRY(117)
ISR_ENTRY(118)
ISR_ENTRY(119)
ISR_ENTRY(120)
ISR_ENTRY(121)
ISR_ENTRY(122)
ISR_ENTRY(123)
ISR_ENTRY(124)
ISR_ENTRY(125)
ISR_ENTRY(126)
ISR_ENTRY(127)
ISR_ENTRY(128)
ISR_ENTRY(129)
ISR_ENTRY(130)
ISR_ENTRY(131)
ISR_ENTRY(132)
ISR_ENTRY(133)
ISR_ENTRY(134)
ISR_ENTRY(135)
ISR_ENTRY(136)
ISR_ENTRY(137)
ISR_ENTRY(138)
ISR_ENTRY(139)
ISR_ENTRY(140)
ISR_ENTRY(141)
ISR_ENTRY(142)
ISR_ENTRY(143)
ISR_ENTRY(144)
ISR_ENTRY(145)
ISR_ENTRY(146)
ISR_ENTRY(147)
ISR_ENTRY(148)
ISR_ENTRY(149)
ISR_ENTRY(150)
ISR_ENTRY(151)
ISR_ENTRY(152)
ISR_ENTRY(153)
ISR_ENTRY(154)
ISR_ENTRY(155)
ISR_ENTRY(156)
ISR_ENTRY(157)
ISR_ENTRY(158)
ISR_ENTRY(159)
ISR_ENTRY(160)
ISR_ENTRY(161)
ISR_ENTRY(162)
ISR_ENTRY(163)
ISR_ENTRY(164)
ISR_ENTRY(165)
ISR_ENTRY(166)
ISR_ENTRY(167)
ISR_ENTRY(168)
ISR_ENTRY(169)
ISR_ENTRY(170)
ISR_ENTRY(171)
ISR_ENTRY(172)
ISR_ENTRY(173)
ISR_ENTRY(174)
ISR_ENTRY(175)
ISR_ENTRY(176)
ISR_ENTRY(177)
ISR_ENTRY(178)
ISR_ENTRY(179)
ISR_ENTRY(180)
ISR_ENTRY(181)
ISR_ENTRY(182)
ISR_ENTRY(183)
ISR_ENTRY(184)
ISR_ENTRY(185)
ISR_ENTRY(186)
ISR_ENTRY(187)
ISR_ENTRY(188)
ISR_ENTRY(189)
ISR_ENTRY(190)
ISR_ENTRY(191)
ISR_ENTRY(192)
ISR_ENTRY(193)
ISR_ENTRY(194)
ISR_ENTRY(195)
ISR_ENTRY(196)
ISR_ENTRY(197)
ISR_ENTRY(198)
ISR_ENTRY(199)
ISR_ENTRY(200)
ISR_ENTRY(201)
ISR_ENTRY(202)
ISR_ENTRY(203)
ISR_ENTRY(204)
ISR_ENTRY(205)
ISR_ENTRY(206)
ISR_ENTRY(207)
ISR_ENTRY(208)
ISR_ENTRY(209)
ISR_ENTRY(210)
ISR_ENTRY(211)
ISR_ENTRY(212)
ISR_ENTRY(213)
ISR_ENTRY(214)
ISR_ENTRY(215)
ISR_ENTRY(216)
ISR_ENTRY(217)
ISR_ENTRY(218)
ISR_ENTRY(219)
ISR_ENTRY(220)
ISR_ENTRY(221)
ISR_ENTRY(222)
ISR_ENTRY(223)
ISR_ENTRY(224)
ISR_ENTRY(225)
ISR_ENTRY(226)
ISR_ENTRY(227)
ISR_ENTRY(228)
ISR_ENTRY(229)
ISR_ENTRY(230)
ISR_ENTRY(231)
ISR_ENTRY(232)
ISR_ENTRY(233)
ISR_ENTRY(234)
ISR_ENTRY(235)
ISR_ENTRY(236)
ISR_ENTRY(237)
ISR_ENTRY(238)
ISR_ENTRY(239)
ISR_ENTRY(240)
ISR_ENTRY(241)
ISR_ENTRY(242)
ISR_ENTRY(243)
ISR_ENTRY(244)
ISR_ENTRY(245)
ISR_ENTRY(246)
ISR_ENTRY(247)
ISR_ENTRY(248)
ISR_ENTRY(249)
ISR_ENTRY(250)
ISR_ENTRY(251)
ISR_ENTRY(252)
ISR_ENTRY(253)
ISR_ENTRY(254)
ISR_ENTRY(255)

	.extern __int_handler

#define SAVE_REGISTERS \
	pushq %rdi; \
	pushq %rsi; \
	pushq %rdx; \
	pushq %rcx; \
	pushq %rax; \
	pushq %r8; \
	pushq %r9; \
	pushq %r10; \
	pushq %r11; \
	pushq %rbx; \
	pushq %rbp; \
	pushq %r12; \
	pushq %r13; \
	pushq %r14; \
	pushq %r15; \


#define LOAD_REGISTERS \
	popq %r15; \
	popq %r14; \
	popq %r13; \
	popq %r12; \
	popq %rbp; \
	popq %rbx; \
	popq %r11; \
	popq %r10; \
	popq %r9; \
	popq %r8; \
	popq %rax; \
	popq %rcx; \
	popq %rdx; \
	popq %rsi; \
	popq %rdi; \


common:
    SAVE_REGISTERS
	cld
	movq %rsp, %rdi
	call __int_handler
    LOAD_REGISTERS
	addq $0x10, %rsp
	iretq

	.global syscall
syscall:
	SAVE_REGISTERS

.extern syscall_table
	movq syscall_table(, %rax, 8), %rbx
	cld
	call *%rbx

end_syscall:
	LOAD_REGISTERS
/*	addq $0x10, %rsp*/
	iretq

	.global __thread_entry
__thread_entry:
	movq %r12, %rdi
	movq %r13, %rsi
	movq %r14, %rdx

	cld
	call thread_entry
	jmp end_syscall


	.global __int_entry
__int_entry:
	.quad isr0
	.quad isr1
	.quad isr2
	.quad isr3
	.quad isr4
	.quad isr5
	.quad isr6
	.quad isr7
	.quad isr8
	.quad isr9
	.quad isr10
	.quad isr11
	.quad isr12
	.quad isr13
	.quad isr14
	.quad isr15
	.quad isr16
	.quad isr17
	.quad isr18
	.quad isr19
	.quad isr20
	.quad isr21
	.quad isr22
	.quad isr23
	.quad isr24
	.quad isr25
	.quad isr26
	.quad isr27
	.quad isr28
	.quad isr29
	.quad isr30
	.quad isr31
	.quad isr32
	.quad isr33
	.quad isr34
	.quad isr35
	.quad isr36
	.quad isr37
	.quad isr38
	.quad isr39
	.quad isr40
	.quad isr41
	.quad isr42
	.quad isr43
	.quad isr44
	.quad isr45
	.quad isr46
	.quad isr47
	.quad isr48
	.quad isr49
	.quad isr50
	.quad isr51
	.quad isr52
	.quad isr53
	.quad isr54
	.quad isr55
	.quad isr56
	.quad isr57
	.quad isr58
	.quad isr59
	.quad isr60
	.quad isr61
	.quad isr62
	.quad isr63
	.quad isr64
	.quad isr65
	.quad isr66
	.quad isr67
	.quad isr68
	.quad isr69
	.quad isr70
	.quad isr71
	.quad isr72
	.quad isr73
	.quad isr74
	.quad isr75
	.quad isr76
	.quad isr77
	.quad isr78
	.quad isr79
	.quad isr80
	.quad isr81
	.quad isr82
	.quad isr83
	.quad isr84
	.quad isr85
	.quad isr86
	.quad isr87
	.quad isr88
	.quad isr89
	.quad isr90
	.quad isr91
	.quad isr92
	.quad isr93
	.quad isr94
	.quad isr95
	.quad isr96
	.quad isr97
	.quad isr98
	.quad isr99
	.quad isr100
	.quad isr101
	.quad isr102
	.quad isr103
	.quad isr104
	.quad isr105
	.quad isr106
	.quad isr107
	.quad isr108
	.quad isr109
	.quad isr110
	.quad isr111
	.quad isr112
	.quad isr113
	.quad isr114
	.quad isr115
	.quad isr116
	.quad isr117
	.quad isr118
	.quad isr119
	.quad isr120
	.quad isr121
	.quad isr122
	.quad isr123
	.quad isr124
	.quad isr125
	.quad isr126
	.quad isr127
	.quad isr128
	.quad isr129
	.quad isr130
	.quad isr131
	.quad isr132
	.quad isr133
	.quad isr134
	.quad isr135
	.quad isr136
	.quad isr137
	.quad isr138
	.quad isr139
	.quad isr140
	.quad isr141
	.quad isr142
	.quad isr143
	.quad isr144
	.quad isr145
	.quad isr146
	.quad isr147
	.quad isr148
	.quad isr149
	.quad isr150
	.quad isr151
	.quad isr152
	.quad isr153
	.quad isr154
	.quad isr155
	.quad isr156
	.quad isr157
	.quad isr158
	.quad isr159
	.quad isr160
	.quad isr161
	.quad isr162
	.quad isr163
	.quad isr164
	.quad isr165
	.quad isr166
	.quad isr167
	.quad isr168
	.quad isr169
	.quad isr170
	.quad isr171
	.quad isr172
	.quad isr173
	.quad isr174
	.quad isr175
	.quad isr176
	.quad isr177
	.quad isr178
	.quad isr179
	.quad isr180
	.quad isr181
	.quad isr182
	.quad isr183
	.quad isr184
	.quad isr185
	.quad isr186
	.quad isr187
	.quad isr188
	.quad isr189
	.quad isr190
	.quad isr191
	.quad isr192
	.quad isr193
	.quad isr194
	.quad isr195
	.quad isr196
	.quad isr197
	.quad isr198
	.quad isr199
	.quad isr200
	.quad isr201
	.quad isr202
	.quad isr203
	.quad isr204
	.quad isr205
	.quad isr206
	.quad isr207
	.quad isr208
	.quad isr209
	.quad isr210
	.quad isr211
	.quad isr212
	.quad isr213
	.quad isr214
	.quad isr215
	.quad isr216
	.quad isr217
	.quad isr218
	.quad isr219
	.quad isr220
	.quad isr221
	.quad isr222
	.quad isr223
	.quad isr224
	.quad isr225
	.quad isr226
	.quad isr227
	.quad isr228
	.quad isr229
	.quad isr230
	.quad isr231
	.quad isr232
	.quad isr233
	.quad isr234
	.quad isr235
	.quad isr236
	.quad isr237
	.quad isr238
	.quad isr239
	.quad isr240
	.quad isr241
	.quad isr242
	.quad isr243
	.quad isr244
	.quad isr245
	.quad isr246
	.quad isr247
	.quad isr248
	.quad isr249
	.quad isr250
	.quad isr251
	.quad isr252
	.quad isr253
	.quad isr254
	.quad isr255
