Disassembly of assigner.bin, 4096 bytes [0x0 to 0x0fff]
coldstart
0000: 4e 9a       JRE     $009C
0002: 00          NOP     
0003: 00          NOP     

NMI handler
0004: aa          EI      
0005: 62          RETI    
0006: 00          NOP     
0007: 00          NOP     

; bit patterns used for Note On
0008: 01 02 04 08 10 20 40 80
; bit patterns used for Note Off
0010: fe fd fb f7 ef df bf 7f

0018: 00 00 00 00 00 00 00 00    

timer handler
0020: 20 cb       INRW    $FFCB_timerCounter
0022: c3          JR      $0026
0023: 54 4b 05    JMP     $054B
0026: aa          EI      
0027: 62          RETI  

serial handler
0028: 54 95 05    JMP     $0595
002b: 00          NOP     
002c: 00          NOP     
002d: 00          NOP     
002e: 00          NOP     
002f: 00          NOP     

table of values referenced at 0x088c
likely to be something to do with
patch data addresses
0030: 9c 97 94 90 9d 9a 95 93
0038: 9e 98 96 91 9b 99 9f 92

LED code numbers for numbers " 1" to "16"
used for MIDI channel display
0040: f1 f2 f3 f4 f5 f6 f7 f8
0048: f9 1a 11 12 13 14 15 16

LED bitfield table for symbols
<top bar> 1 2 3 4 5 6 7
8 9 0 c E P r <space>
0050: 01 60 c7 e5 6c ad af e8
0058: ef ed eb 07 8f ce 07 00

LED bitfield table for transpose values
25 in total
C C. d d. E F F. G G. A A. B
0060: 8b 9b 67 77 8f 8e 9e ab
0068: bb ee fe 2f 8b 9b 67 77
0070: 8f 8e 9e ab bb ee fe 2f 8b

start of code proper
0079: 69 00       MVI     A,$00
007b: 4d c1       MOV     PB,A
007d: 4d c5       MOV     PF,A
007f: 63 cc       STAW    $FFCC  // something to do with ADC
0081: 63 cd       STAW    $FFCD  // something to do with ADC
0083: 63 c0       STAW    $FFC0_runningStatus
0085: 63 b7       STAW    $FFB7  // set to zero for Manual
0087: 64 02 fd    MVI     PC,$FD
008a: aa          EI      
008b: 75 cd 08    EQIW    $FFCD,$08
008e: fc          JR      $008B
008f: 71 b8 2a    MVIW    $FFB8,$2A
0092: 7c 54       CALF    $0C54
0094: 75 cd 10    EQIW    $FFCD,$10
0097: fc          JR      $0094
0098: 7b 81       CALF    $0b81_clrVoiceTbl
009a: 4e 71       JRE     $010D

startup
009c: 69 0e       MVI     A,$0E          // RAM on, 16kB mode, PF6-7 port mode
009e: 4d d0       MOV     MM,A           // set memory map
00a0: 69 0e       MVI     A,$0E          // serial mode clock 24x prescale, RX and TX on
00a2: 4d c9       MOV     SMH,A
00a4: 69 4e       MVI     A,$4E          // 8n1 x16 clock
00a6: 4d ca       MOV     SML,A      
00a8: 64 80 08    MVI     ANM,$08        // select DAC 4-7, scan off, HF mode
00ab: 68 ff       MVI     V,$FF
00ad: 10          EXA     
00ae: 68 ff       MVI     V,$FF          // vector pointer to RAM
00b0: 04 ff ff    LXI     SP,$FFFF       // stack pointer
00b3: 69 00       MVI     A,$00          // clear the RAM
00b5: 34 00 ff    LXI     HL,$FF00
00b8: 3d          STAX    (HL+)
00b9: 74 7f 00    EQI     L,$00
00bc: fb          JR      $00B8
00bd: 69 03       MVI     A,$03          // port C serial on
00bf: 4d d1       MOV     MCC,A
00c1: 69 3a       MVI     A,$3A          // IO bits
00c3: 4d d4       MOV     MC,A   
00c5: 69 00       MVI     A,$00          
00c7: 4d d3       MOV     MB,A           // port B, F set to outputs and turned off
00c9: 4d d7       MOV     MF,A
00cb: 4d c5       MOV     PF,A
00cd: 4d c1       MOV     PB,A
00cf: 69 ff       MVI     A,$FF          // port A set to input
00d1: 4d d2       MOV     MA,A
00d3: 71 bc 08    MVIW    $FFBC,$08      // sustain flag?
00d6: 7b 81       CALF    $0b81_clrVoiceTbl
00d8: 71 c8 42    MVIW    $FFC8_ledCol3,$42      // Bank A Poly 2 
00db: 24 ff 1f    LXI     DE,$1FFF       // enable keypad demux
00de: 4a de       MVIX    DE,$DE         // enable IC9 column 6 for Transpose button
00e0: 4c c0       MOV     A,PA           // port A
00e2: 63 a8       STAW    $FFA8_midiSwitch
00e4: 07 01       ANI     A,$01          // if Transpose was pressed...
00e6: 63 b6       STAW    $FFB6          // set bit 0 of FFB6
00e8: 4a dd       MVIX    DE,$DD         // read slide switches
00ea: 4c c0       MOV     A,PA
00ec: 63 a7       STAW    $FFA7          // store
00ee: 4a e8       MVIX    DE,$E8         // keyboard?
00f0: 64 06 00    MVI     MKH,$00        // unmask ints
00f3: aa          EI
00f4: 75 cd 10    EQIW    $FFCD,$10      // wait for interrupt counter
00f7: fc          JR      $00F4
00f8: 71 b8 2a    MVIW    $FFB8,$2A      // switches1, 8' Square on Saw off Chorus Off
00fb: 7c 47       CALF    $0C47_showManual
00fd: 78 43       CALF    $0843_sendOmniOffPoly
00ff: 71 ba 20    MVIW    $FFBA_bank,$20
0102: 71 bb 01    MVIW    $FFBB_patch,$01
0105: 58 b6       BIT     0,$FFB6 // test mode?        // test mode?
0107: 71 ba 10    MVIW    $FFBA_bank,$10 // if not, set bank
010a: 71 be 0c    MVIW    $FFBE_transpose,$0C    // default transpose is 12
010d: 7b 9a       CALF    $0B9A_clrMidiBits          // 
010f: 78 28       CALF    $0828_centreBend
0111: 58 a8       BIT     0,$FFA8_midiSwitch         // if Transpose is pressed
0113: c7          JR      $011B
0114: 55 87 40    OFFIW   $FF87,$40                  // set if we've previously sent an all notes off message
0117: c3          JR      $011B
0118: 55 a8 08    OFFIW   $FFA8_midiSwitch,$08 (MIDI Ch) // is MIDI Ch pressed?
011b: 4e 75       JRE     $0192                      // yes
011d: 7b 70       CALF    $0B70_stopAllNotes         
011f: 7b 9e       CALF    $0B9E_clrNoteBits
0121: 7b 81       CALF    $0b81_clrVoiceTbl
0123: 7d 33       CALF    $0D33_saveDigitPatt
0125: 78 00       CALF    $0800_showTranspose
0127: 24 ff 1f    LXI     DE,$1FFF       // select MUX
012a: 34 50 ff    LXI     HL,$FF50       // keypad bitfield
012d: 6a e8       MVI     B,$E8          // first column
012f: 0a          MOV     A,B
0130: 3a          STAX    (DE)           // enable
0131: 00          NOP     
0132: 00          NOP     
0133: 4c c0       MOV     A,PA           // read keyboard
0135: 3b          STAX    (HL)           // save in bitfield
0136: 0a          MOV     A,B
0137: 17 10       ORI     A,$10
0139: 3a          STAX    (DE)           // disable column
013a: 32          INX     HL             // next bitfield byte
013b: 42          INR     B              // next column
013c: 48 0b       SK      HC             // rolled over first nybble?
013e: f0          JR      $012F          // no, go back again for the next
013f: 34 50 ff    LXI     HL,$FF50       // keypad bitfield
0142: 6a 07       MVI     B,$07          // 8 bits
0144: 24 00 00    LXI     DE,$0000       //
0147: 2d          LDAX    (HL+)          // fetch bitmask
0148: 67 00       NEI     A,$00          // if it's zero
014a: ca          JR      $0155          // skip past this
014b: 6b 07       MVI     C,$07          // 8 bits
014d: 48 01       SLRC    A              // shift left skip if carry
014f: c1          JR      $0151          // bit was zero
0150: cb          JR      $015C          // bit was one
0151: 22          INX     DE             // bit was zero, increment DE
0152: 53          DCR     C              // count down bits
0153: f9          JR      $014D          // go back for the next bit
0154: c3          JR      $0158          // jump ahead to next bitmask
0155: 74 45 08    ADI     E,$08          // all bits were zero, increment DE by 8
0158: 52          DCR     B              // count down bytes
0159: ed          JR      $0147          // go back for the next bitmask
015a: 4e 21       JRE     $017D          // jump ahead
015c: 0d          MOV     A,E            // save E
015d: 36 0c       SUINB   A,$0C          // less than 12?
015f: 46 0c       ADI     A,$0C          // yes, add 12
0161: 37 25       LTI     A,$25          // less than 37?
0163: 66 0c       SUI     A,$0C          // no, subtract 12
0165: 37 19       LTI     A,$19          // less than 25
0167: 66 0c       SUI     A,$0C          // no, subtract 12
0169: 63 be       STAW    $FFBE_transpose // save transpose value
016b: 77 0c       EQI     A,$0C          // if it's actually 12
016d: c7          JR      $0175          // it isn't
016e: 01 c8       LDAW    $FFC8_ledCol3
0170: 07 fe       ANI     A,$FE          // mask bit
0172: 63 c8       STAW    $FFC8_ledCol3  // turn off Transpose LED
0174: c6          JR      $017B          // jump to display
0175: 01 c8       LDAW    $FFC8_ledCol3  
0177: 17 01       ORI     A,$01          // set bit
0179: 63 c8       STAW    $FFC8_ledCol3  // turn on Transpose LED
017b: 78 00       CALF    $0800_showTranspose
-------------------------------------------------------------
done with transpose?
-------------------------------------------------------------
017d: 24 ff 1f    LXI     DE,$1FFF       // select keypad MUX
0180: 69 de       MVI     A,$DE
0182: 3a          STAX    (DE)           // column 6
0183: 00          NOP     
0184: 00          NOP     
0185: 4c c0       MOV     A,PA           // read it
0187: 47 01       ONI     A,$01          // Transpose?
0189: c2          JR      $018C          // no
018a: 4f 9b       JRE     $0127          // yes, go back to continue transpose routine
018c: 7d 3c       CALF    $0D3C_rstrDigitPatt
018e: 78 28       CALF    $0828_centreBend
0190: 7b 9a       CALF    $0B9A_clrMidiBits
0192: 01 cd       LDAW    $FFCD          // some flag
0194: 07 10       ANI     A,$10          //
0196: 74 f8 bf    EQAW    $FFBF          // some flag
0199: 71 c0 00    MVIW    $FFC0_runningStatus,$00    // clear running status
019c: 63 bf       STAW    $FFBF          // save flag

scan keys again
this populates the key scan table
and also decides if we need to emit some MIDI
019e: 24 ff 1f    LXI     DE,$1FFF       // keypad MUX
01a1: 34 50 ff    LXI     HL,$FF50       // key bitfield
01a4: 6a e8       MVI     B,$E8          // first column
01a6: 0a          MOV     A,B            
01a7: 3a          STAX    (DE)           // enable
01a8: 00          NOP     
01a9: 00          NOP     
01aa: 4c c0       MOV     A,PA           // wait
01ac: 1b          MOV     C,A            // save
01ad: 77 00       EQI     A,$00          // is it zero
01af: 15 87 80    ORIW    $FF87,$80      // yes, set All Notes Off flag
01b2: 0a          MOV     A,B
01b3: 17 10       ORI     A,$10          
01b5: 3a          STAX    (DE)           // disable
01b6: 0b          MOV     A,C            // get bitfield back
01b7: 70 93       XRAX    (HL)           // compare with bitfield already stored
01b9: 48 0c       SK      Z              // has it changed
01bb: 79 96       CALF    $0996          // yes, so we need to do MIDI
01bd: 0b          MOV     A,C            // get bitfield back
01be: 3d          STAX    (HL+)          // save, incrementing HL, in the bitfield RAM
01bf: 42          INR     B              // next column
01c0: 48 0b       SK      HC             // if we rolled over a nybble skip
01c2: e3          JR      $01A6          // go back for the next column
01c3: 55 87 80    OFFIW   $FF87,$80      // there are keys held
01c6: d3          JR      $01DA          // jump ahead
01c7: 5e 87       BIT     6,$FF87        // we already sent All Notes Off
01c9: d0          JR      $01DA          // jump ahead and flag All notes off as Sent
01ca: 69 f9       MVI     A,$F9          
01cc: 79 e8       CALF    $09E8_sendToTx // enable MIDI output
01ce: 69 b0       MVI     A,$B0          // control change
01d0: 79 d5       CALF    $09D5          // send and clear running status
01d2: 69 7b       MVI     A,$7B          // All Notes Off
01d4: 79 e8       CALF    $09E8_sendToTx
01d6: 69 00       MVI     A,$00          // off
01d8: 79 e8       CALF    $09E8_sendToTx
01da: 01 87       LDAW    $FF87          // shift bit 7 to bit 6 to show we've already done it
01dc: 48 21       SLR     A
01de: 63 87       STAW    $FF87

now consider bits from MIDI
01e0: 6a 0d       MVI     B,$0D          // 13 bytes, 104 keys
01e2: 34 3d ff    LXI     HL,$FF3D       // top of note bitfield
01e5: 24 57 ff    LXI     DE,$FF57       // top of keyboard bitfield
01e8: 71 d0 00    MVIW    $FFD0,$00      // zero out
01eb: af 10       LDAX    (HL+$10)       // fetch MIDI bitfield
01ed: 74 3a 03    LTI     B,$03          // B<3, skip
01f0: 74 3a 0b    LTI     B,$0B          // B<11, skip - these select just the 8 bytes of keyboard scan
01f3: c3          JR      $01F7          // it didn't skip, ignore keyboard
01f4: 70 9a       ORAX    (DE)           // combine with the keyboard bitfield
01f6: 23          DCX     DE             // next byte down
01f7: 1b          MOV     C,A            // save
01f8: 70 93       XRAX    (HL)           // compare with current note bitfield
01fa: 48 0c       SK      Z              // has it changed
01fc: 7a 18       CALF    $0A18          // yes, handle that
01fe: 0b          MOV     A,C            // get bitfield back
01ff: 3b          STAX    (HL)           // store in current note bitfield
0200: 33          DCX     HL             // next bitfield down
0201: 52          DCR     B              // count down bitfield bytes
0202: e8          JR      $01EB          // back for the next one
0203: 65 d0 ff    NEIW    $FFD0,$FF      // ffd0 remains a mystery
0206: 7b 9e       CALF    $0B9E_clrNoteBits
-------------------------------------------------------
now consider the keypad buttons
-------------------------------------------------------
0208: 24 ff 1f    LXI     DE,$1FFF       // keypad mux       
020b: 34 a4 ff    LXI     HL,$FFA4       // raw switch data
020e: 6a da       MVI     B,$DA          // mux column, start with bank buttons
0210: 0a          MOV     A,B
0211: 3a          STAX    (DE)           // enable
0212: 00          NOP                    // wait
0213: 00          NOP 
0214: 4c c0       MOV     A,PA           // read
0216: 1b          MOV     C,A            // save
0217: 0a          MOV     A,B
0218: 17 20       ORI     A,$20          // disable
021a: 3a          STAX    (DE)
021b: 0b          MOV     A,C            // get it back
021c: 70 93       XRAX    (HL)           // has it changed?
021e: bf 06       STAX    (HL+$06)       // store xored value
0220: 60 8b       ANA     A,C            // AND with original value
0222: bf 0c       STAX    (HL+$0C)       // store ANDed value
0224: 0b          MOV     A,C            // get value back
0225: 3d          STAX    (HL+)          // store at HL, next byte
0226: 42          INR     B              // next column
0227: 48 0b       SK      HC             // did it roll over nybble
0229: e6          JR      $0210          // no, loop back for next
022a: 58 b6       BIT     0,$FFB6 // test mode?        // test mode?
022c: c2          JR      $022F          // no
022d: 4e 3f       JRE     $026E          // interpret buttons for test mode
022f: 5b ae       BIT     3,$FFAE        // has the MIDI Channel button changed?
0231: d6          JR      $0248          // no
0232: 5b a8       BIT     3,$FFA8_midiSwitch ; MIDI CH button
0234: d1          JR      $0246          // button was released
0235: 78 43       CALF    $0843_sendOmniOffPoly
0237: 7b 9a       CALF    $0B9A_clrMidiBits
0239: 78 28       CALF    $0828_centreBend
023b: 7d 33       CALF    $0D33_saveDigitPatt
023d: 01 bd       LDAW    $FFBD_midiChannel
023f: 34 40 00    LXI     HL,$0040           // lookup table for numbers from " 1" to "16"
0242: ac          LDAX    (HL+A)
0243: 7d 45       CALF    $0D45_digitLookup  // get bit patterns
0245: c2          JR      $0248
0246: 7d 3c       CALF    $0D3C_rstrDigitPatt
0248: 5b a8       BIT     3,$FFA8_midiSwitch ; MIDI CH button
024a: 4e 4f       JRE     $029B  // if it's not still being pressed?
024c: 55 87 40    OFFIW   $FF87,$40  // did we send all notes off?
024f: db          JR      $026B
0250: 64 4a 08    ONI     PC,$08     // sustain pedal
0253: d7          JR      $026B      
0254: 25 b0 00    GTIW    $FFB0,$00  // any bank buttons pressed?
0257: c7          JR      $025F      // none pressed
0258: 01 b0       LDAW    $FFB0
025a: 7c 9d       CALF    $0C9D      // find which button is pressed
025c: 51          DCR     A          // subtract 1, 0-7
025d: 78 13       CALF    $0813      // store MIDI channel number and set flags
025f: 25 b1 00    GTIW    $FFB1,$00  // any patch buttons pressed
0262: c8          JR      $026B      // no
0263: 01 b1       LDAW    $FFB1      // get bitfield
0265: 7c 9d       CALF    $0C9D      // find which button is pressed
0267: 46 07       ADI     A,$07      // add 7, 8-15
0269: 78 13       CALF    $0813      // store MIDI channel number and set flags
026b: 54 5d 04    JMP     $045D      // go on ahead

set up testmode
026e: 5b b4       BIT     3,$FFB4    // MIDI CH pressed?
0270: c2          JR      $0273      // no, go on
0271: 78 5c       CALF    $085C      // tell the module board to zero the DAC
0273: 01 a8       LDAW    $FFA8_midiSwitch
0275: 48 21       SLR     A          // 
0277: 07 38       ANI     A,$38
0279: 1b          MOV     C,A
027a: 01 c8       LDAW    $FFC8_ledCol3
027c: 07 c7       ANI     A,$C7      // mask out load/save/verify
027e: 60 9b       ORA     A,C
0280: 63 c8       STAW    $FFC8_ledCol3  // save it back
0282: 75 ba 10    EQIW    $FFBA_bank,$10
0285: cf          JR      $0295
0286: 59 cd       BIT     1,$FFCD
0288: c7          JR      $0290
0289: 5f b6       BIT     7,$FFB6
028b: 69 30       MVI     A,$30
028d: 7b 55       CALF    $0B55_noteOnUnison
028f: c5          JR      $0295
0290: 5f b6       BIT     7,$FFB6
0292: c2          JR      $0295
0293: 7b 70       CALF    $0B70_stopAllNotes
0295: 25 aa 00    GTIW    $FFAA,$00
0298: c2          JR      $029B
0299: 7b 70       CALF    $0B70_stopAllNotes
029b: 55 b5 38    OFFIW   $FFB5,$38
029e: 54 62 0d    JMP     $0D62
02a1: 59 b5       BIT     1,$FFB5
02a3: c4          JR      $02A8
02a4: 5c a9       BIT     4,$FFA9
02a6: 7c 47       CALF    $0C47_showManual
02a8: 5a af       BIT     2,$FFAF
02aa: 4e 20       JRE     $02CC
02ac: 5a a9       BIT     2,$FFA9
02ae: d0          JR      $02BF
02af: 7d 33       CALF    $0D33_saveDigitPatt
02b1: 05 b6 fb    ANIW    $FFB6,$FB              // omni on/off?
02b4: 64 4a 10    ONI     PC,$10
02b7: cf          JR      $02C7
02b8: 69 00       MVI     A,$00
02ba: 63 b9       STAW    $FFB9
02bc: 7d 45       CALF    $0D45_digitLookup
02be: cd          JR      $02CC
02bf: 5a b6       BIT     2,$FFB6
02c1: 7d 3c       CALF    $0D3C_rstrDigitPatt
02c3: 05 b6 fb    ANIW    $FFB6,$FB              // omni on/off?
02c6: c5          JR      $02CC
02c7: 69 de       MVI     A,$DE
02c9: 40 45 0d    CALL    $0D45_digitLookup
02cc: 5f b4       BIT     7,$FFB4
02ce: 4e 2f       JRE     $02FF
02d0: 01 c8       LDAW    $FFC8_ledCol3
02d2: 16 c0       XRI     A,$C0          // toggle Group A/B LEDs
02d4: 63 c8       STAW    $FFC8_ledCol3  // save it back
02d6: 58 b6       BIT     0,$FFB6 // test mode?        // group button?
02d8: cc          JR      $02E5          // not pressed
02d9: 69 fd       MVI     A,$FD // select module board
02db: 79 e8       CALF    $09E8_sendToTx
02dd: 5e c8       BIT     6,$FFC8_ledCol3    // Group A LED
02df: 69 86       MVI     A,$86              // sustain on/off
02e1: 69 87       MVI     A,$87
02e3: 79 e8       CALF    $09E8_sendToTx
02e5: 59 b6       BIT     1,$FFB6
02e7: 55 a9 04    OFFIW   $FFA9,$04
02ea: d4          JR      $02FF
02eb: 01 bb       LDAW    $FFBB_patch
02ed: 74 98 ba    ORAW    $FFBA_bank
02f0: 1b          MOV     C,A
02f1: 7c d0       CALF    $0CD0
02f3: 7c 93       CALF    $0C93
02f5: 55 a8 10    OFFIW   $FFA8_midiSwitch,$10 (MIDI Func2)
02f8: 79 29       CALF    $0929
02fa: 45 a8 30    ONIW    $FFA8_midiSwitch,$30 (MIDI Func1+2)
02fd: 78 f0       CALF    $08f0_sendSysexPatch
02ff: 45 b4 06    ONIW    $FFB4,$06
0302: d3          JR      $0316
0303: 7b 70       CALF    $0B70_stopAllNotes
0305: 7b 9e       CALF    $0B9E_clrNoteBits
0307: 7b 81       CALF    $0b81_clrVoiceTbl
0309: 01 a8       LDAW    $FFA8_midiSwitch
030b: 07 06       ANI     A,$06              ; Poly mode switches
030d: 1b          MOV     C,A
030e: 01 c8       LDAW    $FFC8_ledCol3      
0310: 07 f9       ANI     A,$F9              ; mask off poly mode LEDs
0312: 60 9b       ORA     A,C                ; OR in switch values
0314: 63 c8       STAW    $FFC8_ledCol3      ; save it back
0316: 25 b1 00    GTIW    $FFB1,$00
0319: 4e 41       JRE     $035C
031b: 01 b1       LDAW    $FFB1
031d: 7c 9d       CALF    $0C9D
031f: 07 0f       ANI     A,$0F
0321: 63 bb       STAW    $FFBB_patch
0323: 5a a9       BIT     2,$FFA9
0325: 4e 20       JRE     $0347
0327: 64 4a 10    ONI     PC,$10
032a: 4e 7a       JRE     $03A6
032c: 5a b6       BIT     2,$FFB6
032e: c2          JR      $0331
032f: 4e 75       JRE     $03A6
0331: 01 b9       LDAW    $FFB9
0333: 07 f0       ANI     A,$F0
0335: 1a          MOV     B,A
0336: 74 98 bb    ORAW    $FFBB_patch
0339: 7d 45       CALF    $0D45_digitLookup
033b: 01 bb       LDAW    $FFBB_patch
033d: 63 b9       STAW    $FFB9
033f: 74 2a 00    GTI     B,$00
0342: d9          JR      $035C
0343: 7d 06       CALF    $0D06
0345: 4e 5f       JRE     $03A6
0347: 74 98 ba    ORAW    $FFBA_bank
034a: 7d 45       CALF    $0D45_digitLookup
034c: 7c 3c       CALF    $0C3C
034e: 7c d0       CALF    $0CD0
0350: 7c 93       CALF    $0C93
0352: 55 a8 10    OFFIW   $FFA8_midiSwitch,$10 (MIDI Func2)
0355: 79 29       CALF    $0929
0357: 45 a8 30    ONIW    $FFA8_midiSwitch,$30 (MIDI Func1+2)
035a: 78 f0       CALF    $08f0_sendSysexPatch
035c: 25 b0 00    GTIW    $FFB0,$00
035f: 4e 45       JRE     $03A6
0361: 01 b0       LDAW    $FFB0
0363: 7c 9d       CALF    $0C9D
0365: 48 25       SLL     A
0367: 48 25       SLL     A
0369: 48 25       SLL     A
036b: 48 25       SLL     A
036d: 63 ba       STAW    $FFBA_bank
036f: 5a a9       BIT     2,$FFA9
0371: df          JR      $0391
0372: 64 4a 10    ONI     PC,$10
0375: 4e 2f       JRE     $03A6
0377: 5a b6       BIT     2,$FFB6
0379: c2          JR      $037C
037a: 4e 2a       JRE     $03A6
037c: 01 b9       LDAW    $FFB9
037e: 07 0f       ANI     A,$0F
0380: 1a          MOV     B,A
0381: 74 98 ba    ORAW    $FFBA_bank
0384: 7d 45       CALF    $0D45_digitLookup
0386: 01 ba       LDAW    $FFBA_bank
0388: 63 b9       STAW    $FFB9
038a: 74 2a 00    GTI     B,$00
038d: d8          JR      $03A6
038e: 7d 06       CALF    $0D06
0390: d5          JR      $03A6
0391: 74 98 bb    ORAW    $FFBB_patch
0394: 7d 45       CALF    $0D45_digitLookup
0396: 7c 3c       CALF    $0C3C
0398: 7c d0       CALF    $0CD0
039a: 7c 93       CALF    $0C93
039c: 55 a8 10    OFFIW   $FFA8_midiSwitch,$10 (MIDI Func2)
039f: 79 29       CALF    $0929
03a1: 45 a8 30    ONIW    $FFA8_midiSwitch,$30 (MIDI Func1+2)
03a4: 78 f0       CALF    $08f0_sendSysexPatch
03a6: 5a a9       BIT     2,$FFA9
03a8: 58 b5       BIT     0,$FFB5
03aa: 4e 28       JRE     $03D4
03ac: 59 b6       BIT     1,$FFB6
03ae: c6          JR      $03B5
03af: 7c 3c       CALF    $0C3C
03b1: 01 bb       LDAW    $FFBB_patch
03b3: 1b          MOV     C,A
03b4: cb          JR      $03C0
03b5: 01 bb       LDAW    $FFBB_patch
03b7: 37 08       LTI     A,$08
03b9: 69 00       MVI     A,$00
03bb: 46 01       ADI     A,$01
03bd: 63 bb       STAW    $FFBB_patch
03bf: 1b          MOV     C,A
03c0: 01 ba       LDAW    $FFBA_bank
03c2: 60 9b       ORA     A,C
03c4: 7d 45       CALF    $0D45_digitLookup
03c6: 7c d0       CALF    $0CD0
03c8: 7c 93       CALF    $0C93
03ca: 55 a8 10    OFFIW   $FFA8_midiSwitch,$10 (MIDI Func2)
03cd: 79 29       CALF    $0929
03cf: 45 a8 30    ONIW    $FFA8_midiSwitch,$30 (MIDI Func1+2)
03d2: 78 f0       CALF    $08f0_sendSysexPatch
03d4: 5f a3       BIT     7,$FFA3_patchVal       // patch set from midi?
03d6: 4e 32       JRE     $040A                  // no, jump ahead
03d8: 05 a3 7f    ANIW    $FFA3_patchVal,$7F     // mask off bit 7
03db: 01 c8       LDAW    $FFC8_ledCol3
03dd: 07 3f       ANI     A,$3F                  // mask
03df: 5e a3       BIT     6,$FFA3_patchVal       // bank
03e1: c3          JR      $03E5                  
03e2: 17 80       ORI     A,$80                  // Group B LED
03e4: c2          JR      $03E7
03e5: 17 40       ORI     A,$40                  // Group A LED
03e7: 63 c8       STAW    $FFC8_ledCol3          // switch on LED
03e9: 01 a3       LDAW    $FFA3_patchVal
03eb: 1b          MOV     C,A
03ec: 07 07       ANI     A,$07
03ee: 41          INR     A
03ef: 63 bb       STAW    $FFBB_patch
03f1: 0b          MOV     A,C
03f2: 07 38       ANI     A,$38
03f4: 48 25       SLL     A
03f6: 46 10       ADI     A,$10
03f8: 63 ba       STAW    $FFBA_bank
03fa: 74 98 bb    ORAW    $FFBB_patch
03fd: 7d 45       CALF    $0D45_digitLookup
03ff: 7c 3c       CALF    $0C3C
0401: 7c d0       CALF    $0CD0
0403: 7c 93       CALF    $0C93
0405: 55 a3 80    OFFIW   $FFA3_patchVal,$80
0408: 4f ce       JRE     $03D8
040a: 4c c2       MOV     A,PC
040c: 1b          MOV     C,A
040d: 74 90 bc    XRAW    $FFBC
0410: 07 08       ANI     A,$08
0412: 48 0c       SK      Z
0414: 7b a9       CALF    $0BA9
0416: 0b          MOV     A,C
0417: 63 bc       STAW    $FFBC
0419: 5b b6       BIT     3,$FFB6
041b: cb          JR      $0427
041c: 05 b6 f7    ANIW    $FFB6,$F7
041f: 69 fd       MVI     A,$FD // select module board // select module board
0421: 79 e8       CALF    $09E8_sendToTx
0423: 69 87       MVI     A,$87
0425: 79 e8       CALF    $09E8_sendToTx
0427: 5c b6       BIT     4,$FFB6
0429: cb          JR      $0435
042a: 05 b6 ef    ANIW    $FFB6,$EF
042d: 69 fd       MVI     A,$FD // select module board // select module board
042f: 79 e8       CALF    $09E8_sendToTx
0431: 69 86       MVI     A,$86
0433: 79 e8       CALF    $09E8_sendToTx
0435: 5f a1       BIT     7,$FFA1
0437: cb          JR      $0443
0438: 01 a1       LDAW    $FFA1
043a: 07 7f       ANI     A,$7F
043c: 63 a1       STAW    $FFA1
043e: 6a a0       MVI     B,$A0
0440: 78 37       CALF    $0837_sendToModule
0442: cd          JR      $0450
0443: 5f a0       BIT     7,$FFA0
0445: ca          JR      $0450
0446: 01 a0       LDAW    $FFA0
0448: 07 7f       ANI     A,$7F
044a: 63 a0       STAW    $FFA0
044c: 6a a1       MVI     B,$A1
044e: 78 37       CALF    $0837_sendToModule
0450: 5f a2       BIT     7,$FFA2
0452: ca          JR      $045D
0453: 01 a2       LDAW    $FFA2
0455: 07 7f       ANI     A,$7F
0457: 63 a2       STAW    $FFA2
0459: 6a a2       MVI     B,$A2
045b: 78 37       CALF    $0837_sendToModule
045d: 25 b2 00    GTIW    $FFB2,$00
0460: 4e 22       JRE     $0484
0462: 59 b6       BIT     1,$FFB6
0464: 7d 5b       CALF    $0D5B
0466: 55 b2 07    OFFIW   $FFB2,$07
0469: 7b cc       CALF    $0BCC
046b: 55 b2 18    OFFIW   $FFB2,$18
046e: 7b da       CALF    $0BDA
0470: 55 b2 e0    OFFIW   $FFB2,$E0
0473: 7b eb       CALF    $0BEB
0475: 7b f9       CALF    $0BF9
0477: 55 a8 30    OFFIW   $FFA8_midiSwitch,$30 (MIDI Func1+2)
047a: c9          JR      $0484
047b: 01 c7       LDAW    $FFC7
047d: 07 7f       ANI     A,$7F
047f: 1b          MOV     C,A
0480: 6d a0       MVI     E,$A0
0482: 78 d6       CALF    $08D6
0484: 25 ad 00    GTIW    $FFAD,$00
0487: d5          JR      $049D
0488: 01 a7       LDAW    $FFA7
048a: 59 b6       BIT     1,$FFB6
048c: 7c 0c       CALF    $0C0C
048e: 7c 29       CALF    $0C29
0490: 55 a8 30    OFFIW   $FFA8_midiSwitch,$30 (MIDI Func1+2)
0493: c9          JR      $049D
0494: 01 8f       LDAW    $FF8F_switches2
0496: 07 1f       ANI     A,$1F
0498: 1b          MOV     C,A
0499: 6d a1       MVI     E,$A1
049b: 78 d6       CALF    $08D6
049d: 6a 00       MVI     B,$00
049f: 24 58 ff    LXI     DE,$FF58
04a2: 34 6c ff    LXI     HL,$FF6C
04a5: 2a          LDAX    (DE)
04a6: 70 b3       SUBNBX  (HL)
04a8: 48 3a       NEGA    
04aa: 19          MOV     EAL,A
04ab: 37 02       LTI     A,$02
04ad: 78 7e       CALF    $087E
04af: 22          INX     DE
04b0: 32          INX     HL
04b1: 42          INR     B
04b2: 74 7a 10    EQI     B,$10
04b5: ef          JR      $04A5
04b6: 2a          LDAX    (DE)
04b7: 70 b3       SUBNBX  (HL)
04b9: 48 3a       NEGA    
04bb: 37 02       LTI     A,$02
04bd: 79 55       CALF    $0955
04bf: 22          INX     DE
04c0: 32          INX     HL
04c1: 01 6b       LDAW    $FF6B
04c3: 27 80       GTI     A,$80
04c5: 69 7f       MVI     A,$7F
04c7: 69 00       MVI     A,$00
04c9: 1b          MOV     C,A
04ca: 74 f8 7f    EQAW    $FF7F
04cd: 79 7e       CALF    $097E
04cf: 55 4f 04    OFFIW   $FF4F,$04
04d2: 54 11 01    JMP     $0111
04d5: 25 ce 00    GTIW    $FFCE,$00
04d8: d7          JR      $04F0
04d9: 7d 5b       CALF    $0D5B
04db: 24 90 ff    LXI     DE,$FF90_patchRam
04de: 6a 07       MVI     B,$07
04e0: 01 ce       LDAW    $FFCE
04e2: 1b          MOV     C,A
04e3: 48 2a       CLC     
04e5: 48 03       SLRC    C
04e7: c2          JR      $04EA
04e8: 79 47       CALF    $0947
04ea: 22          INX     DE
04eb: 52          DCR     B
04ec: f8          JR      $04E5
04ed: 71 ce 00    MVIW    $FFCE,$00
04f0: 25 cf 00    GTIW    $FFCF,$00
04f3: d7          JR      $050B
04f4: 7d 5b       CALF    $0D5B
04f6: 24 98 ff    LXI     DE,$FF98
04f9: 6a 07       MVI     B,$07
04fb: 01 cf       LDAW    $FFCF
04fd: 1b          MOV     C,A
04fe: 48 2a       CLC     
0500: 48 03       SLRC    C
0502: c2          JR      $0505
0503: 79 47       CALF    $0947
0505: 22          INX     DE
0506: 52          DCR     B
0507: f8          JR      $0500
0508: 71 cf 00    MVIW    $FFCF,$00
050b: 5d b6       BIT     5,$FFB6
050d: d0          JR      $051E
050e: 05 b6 df    ANIW    $FFB6,$DF
0511: 7d 5b       CALF    $0D5B
0513: 01 8e       LDAW    $FF8E_switches1
0515: 57 60       OFFI    A,$60
0517: c4          JR      $051C
0518: 17 80       ORI     A,$80
051a: 63 8e       STAW    $FF8E_switches1
051c: 7b f9       CALF    $0BF9
051e: 5e b6       BIT     6,$FFB6
0520: cc          JR      $052D
0521: 05 b6 bf    ANIW    $FFB6,$BF
0524: 7d 5b       CALF    $0D5B
0526: 71 b7 00    MVIW    $FFB7,$00
0529: 01 8f       LDAW    $FF8F_switches2
052b: 7c 29       CALF    $0C29
052d: 58 b6       BIT     0,$FFB6 // test mode?
052f: 54 11 01    JMP     $0111
0532: 6a 05       MVI     B,$05
0534: 34 8d ff    LXI     HL,$FF8D_noteTbl+5
0537: 2f          LDAX    (HL-)
0538: 47 80       ONI     A,$80
053a: c5          JR      $0540
053b: 52          DCR     B
053c: fa          JR      $0537
053d: 69 00       MVI     A,$00
053f: c2          JR      $0542
0540: 0a          MOV     A,B
0541: 41          INR     A
0542: 34 50 00    LXI     HL,$0050
0545: ac          LDAX    (HL+A)
0546: 63 c6       STAW    $FFC6_rightDigit
0548: 54 11 01    JMP     $0111
054b: 10          EXA     
054c: 11          EXX     
054d: 64 c8 08    ONI     ANM,$08
0550: d1          JR      $0562
0551: 34 68 ff    LXI     HL,$FF68
0554: 78 71       CALF    $0871
0556: 01 c5       LDAW    $FFC5_leftDigit
0558: 4d c1       MOV     PB,A
055a: 64 05 00    MVI     PF,$00
055d: 64 80 00    MVI     ANM,$00
0560: 4e 2b       JRE     $058D
0562: 6e ff       MVI     H,$FF
0564: 01 cc       LDAW    $FFCC
0566: 46 58       ADI     A,$58
0568: 1f          MOV     L,A
0569: 78 71       CALF    $0871
056b: 01 cc       LDAW    $FFCC
056d: 46 04       ADI     A,$04
056f: 48 0b       SK      HC
0571: cc          JR      $057E
0572: 71 cc 00    MVIW    $FFCC,$00
0575: 20 cd       INRW    $FFCD
0577: 00          NOP     
0578: 64 80 08    MVI     ANM,$08
057b: 69 fc       MVI     A,$FC
057d: d1          JR      $058F
057e: 63 cc       STAW    $FFCC
0580: 48 01       SLRC    A
0582: 48 01       SLRC    A
0584: 46 c5       ADI     A,$C5
0586: 1f          MOV     L,A
0587: 2b          LDAX    (HL)
0588: 4d c1       MOV     PB,A
058a: 64 45 40    ADI     PF,$40
058d: 69 f7       MVI     A,$F7
058f: 63 cb       STAW    $FFCB_timerCounter_timerCounter
0591: 10          EXA     
0592: 11          EXX     
0593: aa          EI      
0594: 62          RETI

Serial Interrupt Handler
0595: 10          EXA     
0596: 11          EXX     
0597: 64 5e 04    OFFI    MKH,$04    // if the TX interrupt is masked
059a: c5          JR      $05A0      // skip jumping to 05a0
059b: 48 49       SKIT    FSR        // skip if RX interrupt is set, and clear
059d: 54 af 07    JMP     $07AF      // handle the TX interrupt
05a0: 48 6b       SKNIT   ER         // skip if the error flag is not set
05a2: 4e 6f       JRE     $0613      // handle serial error
05a4: 4c d9       MOV     A,RXB      // fetch received byte
05a6: 47 80       ONI     A,$80      // is it a status byte?
05a8: 4e 70       JRE     $061A      // no, handle value
05aa: 37 f0       LTI     A,$F0      // skip if < $f0
05ac: 4e 50       JRE     $05FE      // handle sysex
05ae: 1a          MOV     B,A        // save byte
05af: 05 4f 33    ANIW    $FF4F,$33  // bits 0, 1, 4, 5 left
05b2: 5d 4f       BIT     5,$FF4F    // 
05b4: c4          JR      $05B9      // 
05b5: 74 e0 bd    SUBW    $FFBD_midiChannel      // subtract MIDI channel
05b8: c2          JR      $05BB
05b9: 07 f0       ANI     A,$F0      // mask off channel
05bb: 67 90       NEI     A,$90
05bd: c2          JR      $05C0      // handle Note On
05be: 67 80       NEI     A,$80
05c0: 4e 25       JRE     $05E7      // handle Note Off
05c2: 67 b0       NEI     A,$B0
05c4: d7          JR      $05DC      // handle Control Change
05c5: 5d a8       BIT     5,$FFA8_midiSwitch    // MIDI Func 2
05c7: 4e 2a       JRE     $05F3      // handle patch change etc
05c9: 75 a2 00    EQIW    $FFA2,$00  // these are something to do with pitch bend
05cc: 71 a2 80    MVIW    $FFA2,$80
05cf: 75 a1 00    EQIW    $FFA1,$00
05d2: 71 a1 80    MVIW    $FFA1,$80
05d5: 75 a0 00    EQIW    $FFA0,$00
05d8: 71 a0 80    MVIW    $FFA0,$80
05db: d3          JR      $05EF      // restore registers and return from interrupt      // restore registers and return from interrupt

handle control change
05dc: 1b          MOV     C,A            // save A
05dd: 0a          MOV     A,B            // original status byte
05de: 07 0f       ANI     A,$0F          // mask off MIDI channel
05e0: 74 e8 bd    NEAW    $FFBD_midiChannel
05e3: 15 4f 40    ORIW    $FF4F,$40      // not the right channel
05e6: 0b          MOV     A,C            // restore A
05e7: 71 3f 01    MVIW    $FF3F_midiByteCount,$01  // expect two bytes
05ea: 63 4e       STAW    $FF4E_midiStatusByte      // save status byte
05ec: 15 4f 80    ORIW    $FF4F,$80  // parser status?

return from interrupt
05ef: 10          EXA     
05f0: 11          EXX     
05f1: aa          EI      
05f2: 62          RETI
//

05f3: 67 c0       NEI     A,$C0
05f5: c4          JR      $05FA      // handle patch change
05f6: 77 e0       EQI     A,$E0      // skip if pitchbend?
05f8: f6          JR      $05EF      // restore registers and return from interrupt
05f9: ed          JR      $05E7      // handle pitch bend

expect patch change or sysex
05fa: 71 3f 00    MVIW    $FF3F_midiByteCount,$00  // one additional byte
05fd: ec          JR      $05EA      // store the status byte
05fe: 37 f8       LTI     A,$F8      // anything past sysex end $f7?
0600: ee          JR      $05EF      // restore registers and return from interrupt
0601: 77 f0       EQI     A,$F0
0603: d1          JR      $0615      // not sysex start
0604: 55 a8 30    OFFIW   $FFA8_midiSwitch,$30 (MIDI Func1+2)
0607: cd          JR      $0615      // not in sysex mode
0608: 05 4f f3    ANIW    $FF4F,$F3  
060b: 15 4f 04    ORIW    $FF4F,$04
060e: 71 3f 00    MVIW    $FF3F_midiByteCount,$00  // one additional byte
0611: 4f d7       JRE     $05EA      // store status and return

handle serial error
0613: 4c d9       MOV     A,RXB
0615: 05 4f 33    ANIW    $FF4F,$33
0618: 4f d5       JRE     $05EF      // restore registers and return from interrupt

061a: 5f 4f       BIT     7,$FF4F    // are we looking for a value or status?
061c: fb          JR      $0618      // value, return
061d: 1b          MOV     C,A
061e: 5a 4f       BIT     2,$FF4F
0620: 4e 8f       JRE     $06B1      // multibyte message
0622: 5b 4f       BIT     3,$FF4F
0624: 4e 53       JRE     $0679      // single byte
0626: 5c 4f       BIT     4,$FF4F
0628: 4e 41       JRE     $066B
062a: 01 3f       LDAW    $FF3F_midiByteCount
062c: 37 10       LTI     A,$10      // have we got 16 bytes so far?
062e: 4e 20       JRE     $0650      // jump ahead
0630: 1a          MOV     B,A        // save A
0631: 0b          MOV     A,C        
0632: 34 90 ff    LXI     HL,$FF90_patchRam
0635: bd          STAX    (HL+B)     // store received byte in patch RAM
0636: 34 08 00    LXI     HL,$0008   // address of note bitmask
0639: 0a          MOV     A,B
063a: 07 07       ANI     A,$07      // A = 0 to 7
063c: ac          LDAX    (HL+A)     // a = 01 to 80
063d: 34 cf ff    LXI     HL,$FFCF   //
0640: 74 4a 08    ONI     B,$08      // bit 3 set, why?
0643: 34 ce ff    LXI     HL,$FFCE   // 
0646: 70 9b       ORAX    (HL)       // OR in the bits at that address
0648: 3b          STAX    (HL)       // store it back
0649: 20 3f       INRW    $FF3F_midiByteCount    // next byte
064b: 00          NOP                            // ignore skip
064c: 10          EXA     // return from interrupt
064d: 11          EXX     
064e: aa          EI      
064f: 62          RETI    

save switch bytes from sysex
0650: 67 10       NEI     A,$10          // skip, if we are not at byte 16
0652: ca          JR      $065D          // we are
0653: 67 11       NEI     A,$11          // byte 17
0655: ce          JR      $0664          // we are
0656: 05 4f 33    ANIW    $FF4F,$33      // 18? must be the end of sysex message
0659: 71 3f 00    MVIW    $FF3F_midiByteCount,$00    // reset the counter
065c: ef          JR      $064C          // return from interrupt
065d: 0b          MOV     A,C            
065e: 63 8e       STAW    $FF8E_switches1    // save the switch values
0660: 15 b6 20    ORIW    $FFB6,$20          // set a flag
0663: e5          JR      $0649              // next byte
0664: 0b          MOV     A,C                
0665: 63 8f       STAW    $FF8F_switches2    // save the switches
0667: 15 b6 40    ORIW    $FFB6,$40          // set a flag
066a: f8          JR      $0663              // next byte

reading sysex data
066b: 55 3f 01    OFFIW   $FF3F_midiByteCount,$01    // bit zero set
066e: c3          JR      $0672  // yes
066f: 63 3e       STAW    $FF3E_midiParam1  // no, store count of bytes
0671: f1          JR      $0663              // next byte
0672: 01 3e       LDAW    $FF3E_midiParam1   // get the byte count
0674: 27 11       GTI     A,$11              // >17?
0676: 4f b4       JRE     $062C              // yes, go back and process the sysex packet
0678: ea          JR      $0663              // no, loop around for more bytes

identify sysex data
0679: 75 3f 00    EQIW    $FF3F_midiByteCount,$00 // first byte?
067c: c6          JR      $0683
067d: 77 41       EQI     A,$41                      // yes, is it the Roland identifier?
067f: 4f d5       JRE     $0656                      // abort
0681: 4f c6       JRE     $0649                      // yes, loop around for another byte
0683: 75 3f 01    EQIW    $FF3F_midiByteCount,$01    // second byte?
0686: d4          JR      $069B                      // no
0687: 77 30       EQI     A,$30                      // yes, is it a memory patch?
0689: c5          JR      $068F                      // no
068a: 15 4f 10    ORIW    $FF4F,$10                  // flag that we're receiving a patch dump
068d: 4f ba       JRE     $0649                      // loop around for another byte
068f: 67 31       NEI     A,$31                      // was manual pressed? skip if not
0691: f8          JR      $068A                      // manual message, jump back to flag we're receiving a full patch dump
0692: 77 32       EQI     A,$32                      // was it a single control message?
0694: 4f c0       JRE     $0656                      // no, abort
0696: 05 4f ef    ANIW    $FF4F,$EF                  // mask off patch dump bit
0699: 4f ae       JRE     $0649                      // loop around for another byte

069b: 75 3f 02    EQIW    $FF3F_midiByteCount,$02    // third byte of message
069e: ca          JR      $06A9                      // no
069f: 74 f8 bd    EQAW    $FFBD_midiChannel          // correct MIDI channel?
06a2: 4f b2       JRE     $0656                      // abort
06a4: 55 4f 10    OFFIW   $FF4F,$10                  // single value message
06a7: 4f a0       JRE     $0649                      // no
06a9: 15 4f 08    ORIW    $FF4F,$08                  // set flag
06ac: 71 3f 00    MVIW    $FF3F_midiByteCount,$00    // no more bytes
06af: 4f 9b       JRE     $064C                      // return from interrupt
06b1: 30 3f       DCRW    $FF3F_midiByteCount        // count down, skip if less than zero
06b3: 4e 59       JRE     $070E                      // store it in FF3E, normally param but here byte count
06b5: 25 4e 91    GTIW    $FF4E_midiStatusByte,$91   
06b8: d1          JR      $06CA
06b9: 65 4e b0    NEIW    $FF4E_midiStatusByte,$B0
06bc: 4e 54       JRE     $0712
06be: 65 4e c0    NEIW    $FF4E_midiStatusByte,$C0
06c1: 4e bd       JRE     $0780
06c3: 65 4e e0    NEIW    $FF4E_midiStatusByte,$E0
06c6: 4e c7       JRE     $078F
06c8: 4f 8c       JRE     $0656
-------------------------------------------------
handle note on/off
-------------------------------------------------
06ca: 01 3e       LDAW    $FF3E_midiParam1
06cc: 27 17       GTI     A,$17                      // note < 24
06ce: 4e 36       JRE     $0706                      // yes, add an octave
06d0: 37 6d       LTI     A,$6D                      // note < 109
06d2: 4e 36       JRE     $070A                      // yes, subtract an octave
06d4: 74 e0 be    SUBW    $FFBE_transpose            // subtract the transpose amount
06d7: 1a          MOV     B,A                        // work out bitfield column
06d8: 48 22       SLR     B
06da: 48 22       SLR     B
06dc: 48 22       SLR     B
06de: 07 07       ANI     A,$07                      // 
06e0: 75 4e 90    EQIW    $FF4E_midiStatusByte,$90   // current status is Note On?
06e3: d5          JR      $06F9                      // no, do Note Off
06e4: 74 6b 00    NEI     C,$00                      // velocity is 0?
06e7: d1          JR      $06F9                      // note off
06e8: 34 08 00    LXI     HL,$0008                   // bitfield lookup table
06eb: ac          LDAX    (HL+A)                     // get bit for lower 3 bits of note
06ec: 1b          MOV     C,A                        // store
06ed: 34 40 ff    LXI     HL,$FF40                   // MIDI Note bitfield
06f0: ad          LDAX    (HL+B)                     // fetch the current value
06f1: 60 9b       ORA     A,C                        // OR it in
06f3: bd          STAX    (HL+B)                     // save it back
06f4: 71 3f 01    MVIW    $FF3F_midiByteCount,$01    // expect next message to be two-byte
06f7: 4f 53       JRE     $064C                      // jump back to return from interrupt
06f9: 34 10 00    LXI     HL,$0010                   // table for inverted bitmask
06fc: ac          LDAX    (HL+A)                     // get value
06fd: 1b          MOV     C,A                        // save
06fe: 34 40 ff    LXI     HL,$FF40                   // MIDI note bitfield
0701: ad          LDAX    (HL+B)                     // load the appropriate column
0702: 60 8b       ANA     A,C                        // mask it off
0704: bd          STAX    (HL+B)                     // save it back
0705: ee          JR      $06f4_setup2Byte                      // return from interrupt
0706: 46 0c       ADI     A,$0C                      // add 12 to note
0708: 4f c2       JRE     $06CC                      // jump back
070a: 66 0c       SUI     A,$0C                      // subtract 12 from note
070c: 4f c2       JRE     $06D0                      // jump back
-------------------------------------------------------------------
handle 
-------------------------------------------------------------------
070e: 63 3e       STAW    $FF3E_midiParam1
0710: 4f 3a       JRE     $064C
-------------------------------------------------------------------
handle Control Change message
-------------------------------------------------------------------
responds to All Notes Off, Omni On/Off, Poly On, but not Mono On, apparently
0712: 01 3e       LDAW    $FF3E_midiParam1
0714: 27 7a       GTI     A,$7A                      // greater than 122 = local off?
0716: 4e 35       JRE     $074D                      // no, less
0718: 45 4f 40    ONIW    $FF4F,$40                  // flag
071b: 4f d7       JRE     $06f4_setup2Byte                      // expect 2-byte message, return from interrupt
071d: 67 7b       NEI     A,$7B                      // skip if not equal to All Notes Off
071f: 4e 51       JRE     $0772                      
0721: 47 02       ONI     A,$02                      // bit 1 set?
0723: c8          JR      $072C
0724: 05 4f fd    ANIW    $FF4F,$FD                  // mask bit
0727: 47 01       ONI     A,$01                      // bit 0 set
0729: 4e 4d       JRE     $0778                      // 
072b: c8          JR      $0734
072c: 05 4f fe    ANIW    $FF4F,$FE
072f: 47 01       ONI     A,$01
0731: 15 4f 01    ORIW    $FF4F,$01
0734: 69 03       MVI     A,$03
0736: 74 88 4f    ANAW    $FF4F
0739: 15 4f 20    ORIW    $FF4F,$20
073c: 77 01       EQI     A,$01
073e: 05 4f df    ANIW    $FF4F,$DF
0741: 6a 0d       MVI     B,$0D
0743: 69 00       MVI     A,$00
0745: 34 40 ff    LXI     HL,$FF40
0748: 3d          STAX    (HL+)
0749: 52          DCR     B
074a: fd          JR      $0748
074b: 4f a7       JRE     $06f4_setup2Byte
074d: 77 40       EQI     A,$40                      // is the message Sustain On/Off
074f: ce          JR      $075E                      // no
0750: 74 6b 00    NEI     C,$00                      // skip if value = 0
0753: c5          JR      $0759
0754: 15 b6 10    ORIW    $FFB6,$10                  // sustain off
0757: 4f 9b       JRE     $06f4_setup2Byte
0759: 15 b6 08    ORIW    $FFB6,$08                  // sustain on
075c: 4f 96       JRE     $06f4_setup2Byte                      // set up 2-byte and return from interrupt
075e: 77 01       EQI     A,$01                      // is it modwheel
0760: 4f 92       JRE     $06f4_setup2Byte                      // no, setup 2-byte and return from interrupt
0762: 55 a8 20    OFFIW   $FFA8_midiSwitch,$20 (MIDI Func1)
0765: c7          JR      $076D                      // midi function switch
0766: 0b          MOV     A,C                        // value into a
0767: 17 80       ORI     A,$80                      // OR with $80
0769: 63 a2       STAW    $FFA2                      // store to Modwheel Amount
076b: 4f 87       JRE     $06f4_setup2Byte                      // setup 2-byte, return from interrupt
076d: 71 a2 80    MVIW    $FFA2,$80                  // set modwheel amount to 0
0770: 4f 82       JRE     $06f4_setup2Byte                      // setup 2-byte, return from interrupt
0772: 5d 4f       BIT     5,$FF4F                    
0774: 4f 7e       JRE     $06f4_setup2Byte
0776: 4f c9       JRE     $0741
0778: 74 7b 01    EQI     C,$01
077b: 15 4f 02    ORIW    $FF4F,$02
077e: 4f b4       JRE     $0734
-------------------------------------------------------------------
program change
-------------------------------------------------------------------
0780: 55 a8 20    OFFIW   $FFA8_midiSwitch,$20 (MIDI Func1)
0783: c5          JR      $0789
0784: 0b          MOV     A,C
0785: 17 80       ORI     A,$80                      // bit 7 might be used to indicate the patch needs changed
0787: 63 a3       STAW    $FFA3_patchVal
0789: 71 3f 00    MVIW    $FF3F_midiByteCount,$00
078c: 54 4c 06    JMP     $064C
-------------------------------------------------------------------
pitch bend
-------------------------------------------------------------------
078f: 55 a8 20    OFFIW   $FFA8_midiSwitch,$20 (MIDI Func1)
0792: d7          JR      $07AA              // if it's MIDI mode zero, clear bend and return
0793: 0b          MOV     A,C                // get upper byte
0794: 18          MOV     EAH,A              // into EAH
0795: 01 3e       LDAW    $FF3E_midiParam1   // get lower byte
0797: 48 25       SLL     A                  // left-align it
0799: 19          MOV     EAL,A              // EA now contains bend from midi multiplied by 2
079a: 48 a4       DSLL    EA                 // left-align it
079c: 08          MOV     A,EAH
079d: 47 80       ONI     A,$80              // is it negative?
079f: c4          JR      $07A4
07a0: 63 a1       STAW    $FFA1              // no, store it
07a2: 4f 50       JRE     $06f4_setup2Byte   
07a4: 16 ff       XRI     A,$FF              // invert it
07a6: 63 a0       STAW    $FFA0              // store it in negative bend
07a8: 4f 4a       JRE     $06f4_setup2Byte
07aa: 71 a1 80    MVIW    $FFA1,$80          // just set it to zero
07ad: 4f 45       JRE     $06f4_setup2Byte

handle TX interrupt
07af: 01 c1       LDAW    $FFC1
07b1: 74 f8 c2    EQAW    $FFC2
07b4: 4e 25       JRE     $07DB
07b6: 64 1e 04    ORI     MKH,$04
07b9: 11          EXX     
07ba: 10          EXA     
07bb: aa          EI      
07bc: 62          RETI    


07bd: 1b          MOV     C,A
07be: 4c c2       MOV     A,PC
07c0: 60 93       XRA     A,C
07c2: 47 04       ONI     A,$04
07c4: ca          JR      $07CF
07c5: 69 d0       MVI     A,$D0
07c7: 48 49       SKIT    FSR
07c9: c3          JR      $07CD
07ca: 54 a0 05    JMP     $05A0
07cd: 41          INR     A
07ce: f8          JR      $07C7
07cf: 0f          MOV     A,L
07d0: 37 30       LTI     A,$30
07d2: 69 00       MVI     A,$00
07d4: 63 c1       STAW    $FFC1
07d6: 0b          MOV     A,C
07d7: 4d c2       MOV     PC,A
07d9: 4f d4       JRE     $07AF
07db: 6e ff       MVI     H,$FF
07dd: 1f          MOV     L,A
07de: 2d          LDAX    (HL+)
07df: 67 fd       NEI     A,$FD
07e1: 4f da       JRE     $07BD
07e3: 67 f9       NEI     A,$F9
07e5: 4f d6       JRE     $07BD
07e7: 48 4a       SKIT    FST
07e9: cd          JR      $07F7
07ea: 4d d8       MOV     TXB,A
07ec: 0f          MOV     A,L
07ed: 37 30       LTI     A,$30
07ef: 69 00       MVI     A,$00
07f1: 63 c1       STAW    $FFC1
07f3: 11          EXX     
07f4: 10          EXA     
07f5: aa          EI      
07f6: 62          RETI    
07f7: 33          DCX     HL
07f8: fa          JR      $07F3
07f9: 00          NOP     
07fa: 00          NOP     
07fb: 00          NOP     
07fc: 00          NOP     
07fd: 00          NOP     
07fe: 00          NOP     
07ff: 00          NOP

something to do with display
0800: 01 be       LDAW    $FFBE_transpose
0802: 37 0c       LTI     A,$0C
0804: c4          JR      $0809
0805: 71 c5 04    MVIW    $FFC5_leftDigit,$04
0808: c3          JR      $080C
0809: 71 c5 00    MVIW    $FFC5_leftDigit,$00
080c: 34 60 00    LXI     HL,$0060
080f: ac          LDAX    (HL+A)
0810: 63 c6       STAW    $FFC6_rightDigit
0812: b8          RET

set flags store MIDI channel
0813: 05 4f dc    ANIW    $FF4F,$DC
0816: 15 4f 21    ORIW    $FF4F,$21
0819: 63 bd       STAW    $FFBD_midiChannel
081b: 34 40 00    LXI     HL,$0040
081e: ac          LDAX    (HL+A)
081f: 7d 45       CALF    $0D45_digitLookup
0821: 78 43       CALF    $0843_sendOmniOffPoly
0823: 7b 9a       CALF    $0B9A_clrMidiBits
0825: 78 28       CALF    $0828_centreBend
0827: b8          RET

centre bend to module board
0828: 69 80       MVI     A,$80
082a: 63 a1       STAW    $FFA1
082c: 63 a2       STAW    $FFA2
082e: 69 fd       MVI     A,$FD // select module board
0830: 79 e8       CALF    $09E8_sendToTx
0832: 69 87       MVI     A,$87
0834: 79 e8       CALF    $09E8_sendToTx
0836: b8          RET

send to module board
0837: 1b          MOV     C,A
0838: 69 fd       MVI     A,$FD // select module board
083a: 79 e8       CALF    $09E8_sendToTx
083c: 0a          MOV     A,B
083d: 79 e8       CALF    $09E8_sendToTx
083f: 0b          MOV     A,C
0840: 79 e8       CALF    $09E8_sendToTx
0842: b8          RET

send Omni Off Poly
0843: 69 f9       MVI     A,$F9
0845: 79 e8       CALF    $09E8_sendToTx
0847: 69 b0       MVI     A,$B0
0849: 79 d5       CALF    $09D5
084b: 69 7c       MVI     A,$7C
084d: 79 e8       CALF    $09E8_sendToTx
084f: 69 00       MVI     A,$00
0851: 79 e8       CALF    $09E8_sendToTx
0853: 69 7f       MVI     A,$7F
0855: 79 e8       CALF    $09E8_sendToTx
0857: 69 00       MVI     A,$00
0859: 79 e8       CALF    $09E8_sendToTx
085b: b8          RET     

send Zero DAC command to module board
085c: 69 fd       MVI     A,$FD // select module board
085e: 79 e8       CALF    $09E8_sendToTx
0860: 69 8f       MVI     A,$8F
0862: 79 e8       CALF    $09E8_sendToTx
0864: 69 5f       MVI     A,$5F
0866: 79 e8       CALF    $09E8_sendToTx
0868: 69 a3       MVI     A,$A3
086a: 79 e8       CALF    $09E8_sendToTx
086c: 69 00       MVI     A,$00
086e: 79 e8       CALF    $09E8_sendToTx
0870: b8          RET

read ADCs
0871: 4c e0       MOV     A,CR0
0873: 3d          STAX    (HL+)
0874: 4c e1       MOV     A,CR1
0876: 3d          STAX    (HL+)
0877: 4c e2       MOV     A,CR2
0879: 3d          STAX    (HL+)
087a: 4c e3       MOV     A,CR3
087c: 3b          STAX    (HL)
087d: b8          RET

called with a value in C
save edited value, send to module, and optionally send to sysex
087e: 78 c3       CALF    $08C3
0880: 59 b6       BIT     1,$FFB6                // might be "patch changed" flag or something
0882: 4e 23       JRE     $08A7
0884: 55 c5 10    OFFIW   $FFC5_leftDigit,$10    // if the decimal point is off skip
0887: df          JR      $08A7                  // process notes
0888: b3          PUSH    HL                     // save
0889: b2          PUSH    DE
088a: 6c ff       MVI     D,$FF                  // upper byte of address in RAM
088c: 34 30 00    LXI     HL,$0030               // lookup table in ROM, points to patchRam
088f: ad          LDAX    (HL+B)                 // address
0890: 1d          MOV     E,A                    // lower byte of address in RAM
0891: 0b          MOV     A,C                    
0892: 3a          STAX    (DE)                   // store
0893: 69 fd       MVI     A,$FD // select module board 
0895: 79 e8       CALF    $09E8_sendToTx         // select module board
0897: 0d          MOV     A,E                    // parameter we looked up
0898: 79 e8       CALF    $09E8_sendToTx
089a: 0b          MOV     A,C                    // value in C
089b: 79 e8       CALF    $09E8_sendToTx
089d: 45 a8 30    ONIW    $FFA8_midiSwitch,$30 (MIDI Func1+2)
08a0: 78 d6       CALF    $08D6                  // if sysex is enabled send one value
08a2: a2          POP     DE                     // restore
08a3: a3          POP     HL
08a4: 2a          LDAX    (DE)                   // whatever HL and DE had here
08a5: 3b          STAX    (HL)                   // move from DE to HL
08a6: b8          RET
08a7: b3          PUSH    HL                     // save
08a8: b2          PUSH    DE
08a9: 6c ff       MVI     D,$FF                  // upper byte of address
08ab: 34 30 00    LXI     HL,$0030               // lookup table for patchRam
08ae: ad          LDAX    (HL+B)                 // parameter in B
08af: 1d          MOV     E,A                    // DE now contains pointer to patchRam
08b0: 2a          LDAX    (DE)                   // a now contains value
08b1: 57 80       OFFI    A,$80                  // upper bit set
08b3: c9          JR      $08BD                  // no
08b4: 09          MOV     A,EAL                  // what's in EAL
08b5: 37 10       LTI     A,$10                  // less than 16
08b7: c3          JR      $08BB                  // no
08b8: a2          POP     DE                     // restore and return
08b9: a3          POP     HL
08ba: b8          RET     
08bb: 7d 5b       CALF    $0D5B                  // enable decimal points
08bd: 0b          MOV     A,C                    // set top bit of value
08be: 17 80       ORI     A,$80
08c0: 3a          STAX    (DE)                   // save it back
08c1: 4f d0       JRE     $0893                  // send to module board (which will get the real value from C)

unknown1
08c3: 2a          LDAX    (DE)
08c4: 36 04       SUINB   A,$04
08c6: 69 00       MVI     A,$00
08c8: 27 f2       GTI     A,$F2
08ca: c7          JR      $08D2
08cb: 1b          MOV     C,A
08cc: 66 f2       SUI     A,$F2
08ce: 60 a3       ADDNC   A,C
08d0: 69 ff       MVI     A,$FF
08d2: 48 21       SLR     A
08d4: 1b          MOV     C,A
08d5: b8          RET

send single value sysex
08d6: 6c 32       MVI     D,$32
08d8: 79 15       CALF    $0915_sendSysexHeader
08da: 0d          MOV     A,E
08db: 66 90       SUI     A,$90
08dd: 07 7f       ANI     A,$7F
08df: 79 e8       CALF    $09E8_sendToTx
08e1: 0b          MOV     A,C
08e2: 79 e8       CALF    $09E8_sendToTx
08e4: 69 f7       MVI     A,$F7
08e6: 79 e6       CALF    $09E6_setRunningStatus
08e8: b8          RET

send sysex header for Manual mode
08e9: 6c 31       MVI     D,$31
08eb: 79 15       CALF    $0915_sendSysexHeader
08ed: 69 00       MVI     A,$00
08ef: c6          JR      $08F6

sendSysexPatch
08f0: 6c 30       MVI     D,$30
08f2: 79 15       CALF    $0915_sendSysexHeader
08f4: 79 36       CALF    $0936
08f6: 79 e8       CALF    $09E8_sendToTx
08f8: 6a 0f       MVI     B,$0F
08fa: 34 90 ff    LXI     HL,$FF90_patchRam
08fd: 2d          LDAX    (HL+)
08fe: 07 7f       ANI     A,$7F
0900: 79 e8       CALF    $09E8_sendToTx
0902: 52          DCR     B
0903: f9          JR      $08FD
0904: 01 8e       LDAW    $FF8E_switches1
0906: 07 7f       ANI     A,$7F
0908: 79 e8       CALF    $09E8_sendToTx
090a: 01 8f       LDAW    $FF8F_switches2
090c: 07 1f       ANI     A,$1F
090e: 79 e8       CALF    $09E8_sendToTx
0910: 69 f7       MVI     A,$F7
0912: 79 e8       CALF    $09E8_sendToTx
0914: b8          RET

sendSysexHeader
0915: 69 f9       MVI     A,$F9
0917: 79 e8       CALF    $09E8_sendToTx
0919: 69 f0       MVI     A,$F0
091b: 79 e6       CALF    $09E6_setRunningStatus
091d: 69 41       MVI     A,$41
091f: 79 e8       CALF    $09E8_sendToTx
0921: 0c          MOV     A,D
0922: 79 e8       CALF    $09E8_sendToTx
0924: 01 bd       LDAW    $FFBD_midiChannel
0926: 79 e8       CALF    $09E8_sendToTx
0928: b8          RET

send Program Change message over MIDI
0929: 69 f9       MVI     A,$F9
092b: 79 e8       CALF    $09E8_sendToTx
092d: 69 c0       MVI     A,$C0
092f: 79 d5       CALF    $09D5
0931: 79 36       CALF    $0936
0933: 79 e8       CALF    $09E8_sendToTx
0935: b8          RET

calculate patch number for MIDI
0936: 01 bb       LDAW    $FFBB_patch
0938: 51          DCR     A
0939: 1b          MOV     C,A
093a: 01 ba       LDAW    $FFBA_bank
093c: 66 10       SUI     A,$10
093e: 48 21       SLR     A
0940: 60 9b       ORA     A,C
0942: 5e c8       BIT     6,$FFC8_ledCol3
0944: 17 40       ORI     A,$40
0946: b8          RET

send E followed by (DE) to module board
0947: 69 fd       MVI     A,$FD // select module board
0949: 79 e8       CALF    $09E8_sendToTx
094b: 0d          MOV     A,E
094c: 79 e8       CALF    $09E8_sendToTx
094e: 2a          LDAX    (DE)
094f: 07 7f       ANI     A,$7F
0951: 3a          STAX    (DE)
0952: 79 e8       CALF    $09E8_sendToTx
0954: b8          RET

send Pitch Bend
0955: 55 a8 20    OFFIW   $FFA8_midiSwitch,$20 (MIDI Func1)
0958: b8          RET     
0959: 2a          LDAX    (DE)
095a: 3b          STAX    (HL)
095b: 44 00 00    LXI     EA,$0000
095e: 35 6a 80    LTIW    $FF6A,$80
0961: 16 ff       XRI     A,$FF
0963: 18          MOV     EAH,A
0964: 48 a0       DSLR    EA
0966: 48 a0       DSLR    EA
0968: 69 f9       MVI     A,$F9
096a: 79 e8       CALF    $09E8_sendToTx
096c: 69 e0       MVI     A,$E0
096e: 79 dc       CALF    $09DC
0970: 09          MOV     A,EAL
0971: 48 21       SLR     A
0973: 79 e8       CALF    $09E8_sendToTx
0975: 08          MOV     A,EAH
0976: 25 6a 80    GTIW    $FF6A,$80
0979: 17 40       ORI     A,$40
097b: 79 e8       CALF    $09E8_sendToTx
097d: b8          RET

send modwheel to MIDI
097e: 0b          MOV     A,C
097f: 63 7f       STAW    $FF7F
0981: 55 a8 20    OFFIW   $FFA8_midiSwitch,$20 (MIDI Func1)
0984: b8          RET     
0985: 1b          MOV     C,A
0986: 69 f9       MVI     A,$F9
0988: 79 e8       CALF    $09E8_sendToTx
098a: 69 b0       MVI     A,$B0
098c: 79 d5       CALF    $09D5
098e: 69 01       MVI     A,$01
0990: 79 e8       CALF    $09E8_sendToTx
0992: 0b          MOV     A,C
0993: 79 e8       CALF    $09E8_sendToTx
0995: b8          RET

handles note on/off from key bitfield
sends velocity byte after note on
called with column in B, bitfield in C, difference in A
0996: 19          MOV     EAL,A          // save difference
0997: 0a          MOV     A,B            
0998: 07 07       ANI     A,$07
099a: 7a 0d       CALF    $0A0D          // routine that multiplies A by 8 and saves it in EAH then stores C to FFC3
099c: b3          PUSH    HL
099d: b1          PUSH    BC
099e: 6a 00       MVI     B,$00          //
09a0: 09          MOV     A,EAL          // difference
09a1: 1b          MOV     C,A            // into C
09a2: 48 03       SLRC    C              // shift right, skip if carry
09a4: d5          JR      $09BA          // bit 7 was 0
09a5: 34 08 00    LXI     HL,$0008       // bit 7 was 1
09a8: ad          LDAX    (HL+B)         // get bit value
09a9: 74 c8 c3    ONAW    $FFC3          // is that bit set in the saved bitfield
09ac: c7          JR      $09B4          // no
09ad: 79 c2       CALF    $09C2          // send Note On to MIDI
09af: 69 40       MVI     A,$40          // velocity is 64
09b1: 79 e8       CALF    $09E8_sendToTx
09b3: c6          JR      $09BA          // loop around to next bit
09b4: 79 c2       CALF    $09C2          // Note Off is send Note On to MIDI 
09b6: 69 00       MVI     A,$00          // but with velocity 0
09b8: 79 e8       CALF    $09E8_sendToTx
09ba: 42          INR     B              // next nit
09bb: 74 7a 08    EQI     B,$08          // if we've done all 8
09be: e3          JR      $09A2          // we haven't, loop
09bf: a1          POP     BC             // restore and return
09c0: a3          POP     HL
09c1: b8          RET

send Note On to MIDI
09c2: 69 f9       MVI     A,$F9
09c4: 79 e8       CALF    $09E8_sendToTx
09c6: 69 90       MVI     A,$90
09c8: 79 dc       CALF    $09DC
09ca: 08          MOV     A,EAH  // contains column number *8
09cb: 60 9a       ORA     A,B    // contains bit number count (lower 3 bits) of note
09cd: 46 18       ADI     A,$18  // +24
09cf: 74 c0 be    ADDW    $FFBE_transpose    // +transpose
09d2: 79 e8       CALF    $09E8_sendToTx
09d4: b8          RET

add MIDI channel to status, set running status, send
09d5: 74 c0 bd    ADDW    $FFBD_midiChannel
09d8: 71 c0 00    MVIW    $FFC0_runningStatus,$00
09db: cc          JR      $09E8_sendToTx

add MIDI channel and send
09dc: 47 80       ONI     A,$80                  // is it a status byte
09de: c9          JR      $09E8_sendToTx         // no, just send it
09df: 74 c0 bd    ADDW    $FFBD_midiChannel      // yes, add the MIDI channel
09e2: 74 e8 c0    NEAW    $FFC0_runningStatus    // skip if not the same as the current running status
09e5: b8          RET                            // yes it is, return

set running status
09e6: 63 c0       STAW    $FFC0_runningStatus    // save the running status and fall through to
send to TX
09e8: b3          PUSH    HL                     // send to TX
09e9: b1          PUSH    BC
09ea: b2          PUSH    DE
09eb: 1b          MOV     C,A
09ec: 01 c2       LDAW    $FFC2
09ee: 41          INR     A
09ef: 67 30       NEI     A,$30
09f1: 69 00       MVI     A,$00
09f3: 74 e8 c1    NEAW    $FFC1
09f6: fc          JR      $09F3
09f7: 1d          MOV     E,A
09f8: 01 c2       LDAW    $FFC2
09fa: 34 00 ff    LXI     HL,$FF00
09fd: 1a          MOV     B,A
09fe: 0b          MOV     A,C
09ff: bd          STAX    (HL+B)
0a00: 0d          MOV     A,E
0a01: 64 1e 07    ORI     MKH,$07
0a04: 63 c2       STAW    $FFC2
0a06: 64 0e f8    ANI     MKH,$F8
0a09: a2          POP     DE
0a0a: a1          POP     BC
0a0b: a3          POP     HL
0a0c: b8          RET

set up to calculate note from bitfield
given one bit from the key on bitfield, use the column *8
which ends up in EAH and C (the original bitfield) ends up
in FFC3 where it's XORed to see if a note has changed
0a0d: 48 25       SLL     A
0a0f: 48 25       SLL     A
0a11: 48 25       SLL     A
0a13: 18          MOV     EAH,A
0a14: 0b          MOV     A,C
0a15: 63 c3       STAW    $FFC3
0a17: b8          RET

voice on/off to module board
when we enter here A contains the "changed" bits
found by XORing the "calculated" bitfield with
the "stored" bitfield
0a18: 19          MOV     EAL,A          // save 
0a19: 0a          MOV     A,B            
0a1a: 7a 0d       CALF    $0A0D          // construct note from B and C
0a1c: b3          PUSH    HL
0a1d: b1          PUSH    BC
0a1e: 6a 07       MVI     B,$07          // 8 bits
0a20: 09          MOV     A,EAL          // note bitfield         
0a21: 1b          MOV     C,A            
0a22: 34 08 00    LXI     HL,$0008       // lookup table of bits
0a25: 48 07       SLLC    C              // shift bitfield out
0a27: 4e 37       JRE     $0A60          // skip this on no carry
0a29: ad          LDAX    (HL+B)         // found a bit, look it up in the list
0a2a: 74 c8 c3    ONAW    $FFC3          // was it already set?
0a2d: 4e 23       JRE     $0A52          // Yes
0a2f: 08          MOV     A,EAH          // No, note on
0a30: 60 9a       ORA     A,B            // combine B with EAH, which holds the upper four bits of note number
0a32: 59 c8       BIT     1,$FFC8_ledCol3        // Poly 1
0a34: da          JR      $0A4F          // poly 1 not set, jump to poly 2 assigner
0a35: 5a c8       BIT     2,$FFC8_ledCol3        // Poly 2
0a37: c7          JR      $0A3F          // poly 2 not set, jump to poly 1 assigner
0a38: 58 b6       BIT     0,$FFB6 // test mode?        // unison mode, get test mode
0a3a: c7          JR      $0A42          // not test mode, skip
0a3b: 7a 88       CALF    $0A88          // cycle round voices
0a3d: 4e 21       JRE     $0A60          // jump on and do next note
-----------------------------------------------------------
Poly 1 Note On handler
-----------------------------------------------------------
0a3f: 58 b6       BIT     0,$FFB6 // test mode?        // test mode?
0a41: ca          JR      $0A4C          // no
0a42: 75 d0 00    EQIW    $FFD0,$00      // try next note if there's already a note held
0a45: da          JR      $0A60          // do next note
0a46: 7b 55       CALF    $0B55_noteOnUnison
0a48: 71 d0 80    MVIW    $FFD0,$80      // set flag for note held
0a4b: d4          JR      $0A60          // do next note
0a4c: 7a af       CALF    $0AAF          // real poly1 assigner
0a4e: d1          JR      $0A60          // do next note
-----------------------------------------------------------
Poly 2 Note On handler
-----------------------------------------------------------
0a4f: 7a 76       CALF    $0A76          // real poly 2 assigner works the same in test and normal mode
0a51: ce          JR      $0A60          // do the next note
-----------------------------------------------------------
Main Note Off handler
-----------------------------------------------------------
0a52: 08          MOV     A,EAH
0a53: 60 9a       ORA     A,B                // get true note number
0a55: 59 c8       BIT     1,$FFC8_ledCol3    // Poly 1
0a57: c6          JR      $0A5E              // no, go to poly2 handler
0a58: 5a c8       BIT     2,$FFC8_ledCol3    // Poly 2
0a5a: cb          JR      $0A66              // no, go to poly1 handler
0a5b: 58 b6       BIT     0,$FFB6 // test mode?
0a5d: cb          JR      $0A69              // unison handler
0a5e: 7b 30       CALF    $0B30              // test mode cyclic turn off voices
-----------------------------------------------------------
0a60: 52          DCR     B                  // count down
0a61: 4f c2       JRE     $0A25              // if there are bits left loop back
0a63: a1          POP     BC                 // return
0a64: a3          POP     HL
0a65: b8          RET     
-----------------------------------------------------------
test mode "poly1" note off
-----------------------------------------------------------
0a66: 58 b6       BIT     0,$FFB6 // test mode?
0a68: ca          JR      $0A73                  // no
0a69: 75 d0 00    EQIW    $FFD0,$00              // no note held
0a6c: f3          JR      $0A60                  // next note in byte
0a6d: 7b 70       CALF    $0B70_stopAllNotes 
0a6f: 71 d0 ff    MVIW    $FFD0,$FF              // this flag
0a72: ed          JR      $0A60                  // loop back
0a73: 7a f8       CALF    $0AF8                  // real Poly 1 handler
0a75: ea          JR      $0A60

-----------------------------------------------------------
real Poly 2 assigner
-----------------------------------------------------------
voice functions
0a76: b3          PUSH    HL
0a77: b1          PUSH    BC                     // save
0a78: 1b          MOV     C,A                    // changed bit
0a79: 6a 05       MVI     B,$05                  // six voices
0a7b: 34 88 ff    LXI     HL,$FF88_noteTbl               // voice table
0a7e: 2b          LDAX    (HL)                   // get value
0a7f: 57 80       OFFI    A,$80                  // top bit clear
0a81: dc          JR      $0A9E                  // no, send note to module board
0a82: 32          INX     HL                     // next voice
0a83: 52          DCR     B                      // count down
0a84: f9          JR      $0A7E                  // loop back
0a85: a1          POP     BC                     // return
0a86: a3          POP     HL
0a87: b8          RET

test mode for "Unison", cycle round voices
0a88: b3          PUSH    HL
0a89: b1          PUSH    BC
0a8a: 6e ff       MVI     H,$FF
0a8c: 1b          MOV     C,A
0a8d: 01 86       LDAW    $FF86
0a8f: 41          INR     A
0a90: 77 8e       EQI     A,$8E
0a92: c2          JR      $0A95
0a93: 69 88       MVI     A,$88
0a95: 63 86       STAW    $FF86
0a97: 1f          MOV     L,A
0a98: 2b          LDAX    (HL)
0a99: 57 80       OFFI    A,$80
0a9b: c2          JR      $0A9E
0a9c: 52          DCR     B
0a9d: ef          JR      $0A8D
-----------------------------------------------------------
called with note number in L, note to play in C
-----------------------------------------------------------
0a9e: 69 fd       MVI     A,$FD // select module board
0aa0: 79 e8       CALF    $09E8_sendToTx
0aa2: 0f          MOV     A,L
0aa3: 79 e8       CALF    $09E8_sendToTx
0aa5: 0b          MOV     A,C
0aa6: 3b          STAX    (HL)
0aa7: 74 c0 be    ADDW    $FFBE_transpose
0aaa: 79 e8       CALF    $09E8_sendToTx
0aac: a1          POP     BC
0aad: a3          POP     HL
0aae: b8          RET  

-----------------------------------------------------------
real Poly 1 note on assigner
-----------------------------------------------------------
0aaf: b3          PUSH    HL
0ab0: b1          PUSH    BC
0ab1: b2          PUSH    DE
0ab2: 1b          MOV     C,A                // save all registers including A
0ab3: 6a 05       MVI     B,$05              // six voices
0ab5: 6c ff       MVI     D,$FF              // upper byte of pointer
0ab7: 34 85 ff    LXI     HL,$FF80_voiceTbl+5
0aba: 2b          LDAX    (HL)               // get voice
0abb: 1d          MOV     E,A                // save
0abc: 2a          LDAX    (DE)               // get value pointed to from note table
0abd: 57 80       OFFI    A,$80              // bit7?
0abf: ca          JR      $0ACA              // is set
0ac0: a2          POP     DE                 // return, is this "all notes used?"
0ac1: a1          POP     BC
0ac2: a3          POP     HL
0ac3: b8          RET     
0ac4: 2b          LDAX    (HL)               // fetch next voice
0ac5: 1d          MOV     E,A                // form address
0ac6: 2a          LDAX    (DE)               // fetch note number
0ac7: 47 80       ONI     A,$80              // test if top bit is set
0ac9: c8          JR      $0AD2              // no, jump to tried all voices?
0aca: 07 7f       ANI     A,$7F              // mask off top bit
0acc: 60 eb       NEA     A,C                // is this the note we're already playing
0ace: d1          JR      $0AE0              // not equal, go ahead
0acf: 33          DCX     HL                 // count down a voice
0ad0: 52          DCR     B                  // one less to try
0ad1: f2          JR      $0AC4              // loop back around to slightly before the beginning
0ad2: 32          INX     HL                 // tried all voices, back up one
0ad3: 2b          LDAX    (HL)               // get voice number
0ad4: 1d          MOV     E,A                // save
0ad5: 0b          MOV     A,C                // note number in A, voice number in E
0ad6: ce          JR      $0AE5              // jump ahead to store and send
0ad7: 33          DCX     HL                 // count down
0ad8: af 01       LDAX    (HL+$01)           // get voice in next slot up
0ada: 1b          MOV     C,A                // save
0adb: 2b          LDAX    (HL)               // get this voice
0adc: bf 01       STAX    (HL+$01)           // save in next slot up
0ade: 0b          MOV     A,C                // restore
0adf: 3b          STAX    (HL)               // swap voices
0ae0: 52          DCR     B                  // count down voices to try
0ae1: f5          JR      $0AD7              // loop back around
0ae2: 2a          LDAX    (DE)               // ran out of voices, get note
0ae3: 07 7f       ANI     A,$7F              // mask off top bit
0ae5: 3a          STAX    (DE)               // save it back
0ae6: 74 c0 be    ADDW    $FFBE_transpose    // add transpose
0ae9: 1b          MOV     C,A                // save to C
0aea: 69 fd       MVI     A,$FD // select module board
0aec: 79 e8       CALF    $09E8_sendToTx
0aee: 0d          MOV     A,E                // voice number
0aef: 79 e8       CALF    $09E8_sendToTx
0af1: 0b          MOV     A,C                // note number
0af2: 79 e8       CALF    $09E8_sendToTx
0af4: a2          POP     DE                 // unstack and return
0af5: a1          POP     BC
0af6: a3          POP     HL
0af7: b8          RET

-----------------------------------------------------------
Real Poly 1 Note Off assigner
-----------------------------------------------------------
0af8: b3          PUSH    HL
0af9: b1          PUSH    BC
0afa: b2          PUSH    DE
0afb: 1b          MOV     C,A                // save all
0afc: 6a 05       MVI     B,$05              // six voices
0afe: 6c ff       MVI     D,$FF              // set up DE as a pointer
0b00: 34 80 ff    LXI     HL,$FF80_voiceTbl
0b03: 2b          LDAX    (HL)               // voice number
0b04: 1d          MOV     E,A                // DE points to note table
0b05: 2a          LDAX    (DE)               // get the note that's being played
0b06: 07 7f       ANI     A,$7F              // mask off the play/stop bit
0b08: 60 eb       NEA     A,C                // A != C?
0b0a: d0          JR      $0B1B              // found it
0b0b: 32          INX     HL                 // next voice 
0b0c: 52          DCR     B                  // count down
0b0d: f5          JR      $0B03              // loop
0b0e: a2          POP     DE                 // return
0b0f: a1          POP     BC
0b10: a3          POP     HL
0b11: b8          RET     
0b12: 2b          LDAX    (HL)
0b13: 1b          MOV     C,A
0b14: af 01       LDAX    (HL+$01)
0b16: 3b          STAX    (HL)
0b17: 0b          MOV     A,C
0b18: bf 01       STAX    (HL+$01)
0b1a: 32          INX     HL
0b1b: 52          DCR     B                  // count down B anyway
0b1c: f5          JR      $0B12              // if we're not done yet loop
0b1d: 2a          LDAX    (DE)               // get note
0b1e: 17 80       ORI     A,$80              // OR in "note off"
0b20: 3a          STAX    (DE)               // save it back
0b21: 0d          MOV     A,E                // 
0b22: 66 08       SUI     A,$08              // subtract 8 from the voice number
0b24: 1b          MOV     C,A                // to put the correct command value in C
0b25: 69 fd       MVI     A,$FD // select module board
0b27: 79 e8       CALF    $09E8_sendToTx
0b29: 0b          MOV     A,C
0b2a: 79 e8       CALF    $09E8_sendToTx     // send 0x80 to 0x85 to the module board
0b2c: a2          POP     DE
0b2d: a1          POP     BC
0b2e: a3          POP     HL
0b2f: b8          RET

-----------------------------------------------------------
test mode voice off cyclic thing
-----------------------------------------------------------
0b30: b3          PUSH    HL
0b31: b1          PUSH    BC
0b32: 1b          MOV     C,A
0b33: 6a 05       MVI     B,$05
0b35: 34 88 ff    LXI     HL,$FF88_noteTbl
0b38: 2b          LDAX    (HL)
0b39: 47 80       ONI     A,$80
0b3b: c6          JR      $0B42
0b3c: 32          INX     HL
0b3d: 52          DCR     B
0b3e: f9          JR      $0B38
0b3f: a1          POP     BC
0b40: a3          POP     HL
0b41: b8          RET     
0b42: 60 eb       NEA     A,C
0b44: c1          JR      $0B46
0b45: f6          JR      $0B3C
0b46: 17 80       ORI     A,$80
0b48: 3b          STAX    (HL)
0b49: 69 fd       MVI     A,$FD // select module board
0b4b: 79 e8       CALF    $09E8_sendToTx
0b4d: 0f          MOV     A,L
0b4e: 66 08       SUI     A,$08
0b50: 79 e8       CALF    $09E8_sendToTx
0b52: a1          POP     BC
0b53: a3          POP     HL
0b54: b8          RET

send unison note on to module board
note in A
0b55: b3          PUSH    HL                 // save
0b56: b1          PUSH    BC
0b57: 74 c0 be    ADDW    $FFBE_transpose
0b5a: 1b          MOV     C,A
0b5b: 6a 05       MVI     B,$05              // six voices
0b5d: 34 88 ff    LXI     HL,$FF88_noteTbl           // note table
0b60: 69 fd       MVI     A,$FD // select module board
0b62: 79 e8       CALF    $09E8_sendToTx
0b64: 0f          MOV     A,L                // voice on
0b65: 79 e8       CALF    $09E8_sendToTx
0b67: 0b          MOV     A,C                // note
0b68: 79 e8       CALF    $09E8_sendToTx
0b6a: 32          INX     HL                 // next voice
0b6b: 52          DCR     B                  // count down
0b6c: f7          JR      $0B64              // loop around for the rest
0b6d: a1          POP     BC                 // restore and return
0b6e: a3          POP     HL
0b6f: b8          RET     

stop all notes
0b70: b1          PUSH    BC
0b71: 6b 80       MVI     C,$80          // first "voice off"
0b73: 6a 05       MVI     B,$05          // six voices
0b75: 69 fd       MVI     A,$FD // select module board          // send to module board
0b77: 79 e8       CALF    $09E8_sendToTx
0b79: 0b          MOV     A,C
0b7a: 79 e8       CALF    $09E8_sendToTx
0b7c: 43          INR     C
0b7d: 52          DCR     B
0b7e: fa          JR      $0B79
0b7f: a1          POP     BC
0b80: b8          RET 

set up new voice allocation table
0b81: 34 88 ff    LXI     HL,$FF88_noteTbl   // $FF88_noteTbl contains six bytes of $80
0b84: 6a 05       MVI     B,$05
0b86: 69 80       MVI     A,$80
0b88: 3d          STAX    (HL+)
0b89: 52          DCR     B
0b8a: fd          JR      $0B88
0b8b: 34 80 ff    LXI     HL,$FF80_voiceTbl   // $FF80_voiceTbl contains $88 to $8D
0b8e: 6a 05       MVI     B,$05
0b90: 69 88       MVI     A,$88
0b92: 3d          STAX    (HL+)
0b93: 41          INR     A
0b94: 52          DCR     B
0b95: fc          JR      $0B92
0b96: 71 86 88    MVIW    $FF86,$88  // finally $FF86 contains $88, first voice
0b99: b8          RET

clear MIDI on bits
0b9a: 34 40 ff    LXI     HL,$FF40
0b9d: c3          JR      $0BA1

clear note on bits
0b9e: 34 30 ff    LXI     HL,$FF30
0ba1: 6a 0d       MVI     B,$0D
0ba3: 69 00       MVI     A,$00
0ba5: 3d          STAX    (HL+)
0ba6: 52          DCR     B
0ba7: fd          JR      $0BA5
0ba8: b8          RET

Send sustain on/off to module board
called with bit 3 in C set for on and cleared for off
0ba9: 69 fd       MVI     A,$FD // select module board // select module board
0bab: 79 e8       CALF    $09E8_sendToTx
0bad: 0b          MOV     A,C
0bae: 47 08       ONI     A,$08
0bb0: 69 86       MVI     A,$86              // module sustain on
0bb2: 69 87       MVI     A,$87              // module sustain off
0bb4: 79 e8       CALF    $09E8_sendToTx     // send to module
0bb6: 69 f9       MVI     A,$F9
0bb8: 79 e8       CALF    $09E8_sendToTx     // select MIDI
0bba: 69 b0       MVI     A,$B0              // control change
0bbc: 79 d5       CALF    $09D5              // send, zero out running status
0bbe: 69 40       MVI     A,$40              // Sustain CC
0bc0: 79 e8       CALF    $09E8_sendToTx     // send
0bc2: 0b          MOV     A,C                // get value again
0bc3: 47 08       ONI     A,$08
0bc5: 69 7f       MVI     A,$7F              // max
0bc7: 69 00       MVI     A,$00              // min
0bc9: 79 e8       CALF    $09E8_sendToTx     // send
0bcb: b8          RET

range switches
0bcc: 01 b2       LDAW    $FFB2
0bce: 07 07       ANI     A,$07
0bd0: 1a          MOV     B,A
0bd1: 01 8e       LDAW    $FF8E_switches1
0bd3: 07 f8       ANI     A,$F8
0bd5: 60 9a       ORA     A,B
0bd7: 63 8e       STAW    $FF8E_switches1
0bd9: b8          RET

osc switches
0bda: 01 b2       LDAW    $FFB2
0bdc: 74 90 8e    XRAW    $FF8E_switches1
0bdf: 07 18       ANI     A,$18
0be1: 1a          MOV     B,A
0be2: 01 8e       LDAW    $FF8E_switches1
0be4: 07 e7       ANI     A,$E7
0be6: 60 9a       ORA     A,B
0be8: 63 8e       STAW    $FF8E_switches1
0bea: b8          RET

range and osc switches
0beb: 01 b2       LDAW    $FFB2
0bed: 07 e0       ANI     A,$E0
0bef: 1a          MOV     B,A
0bf0: 01 8e       LDAW    $FF8E_switches1
0bf2: 07 1f       ANI     A,$1F
0bf4: 60 9a       ORA     A,B
0bf6: 63 8e       STAW    $FF8E_switches1
0bf8: b8          RET

send switches1 to module board
0bf9: 63 c7       STAW    $FFC7
0bfb: 16 ff       XRI     A,$FF
0bfd: 07 7f       ANI     A,$7F
0bff: 1b          MOV     C,A
0c00: 69 fd       MVI     A,$FD // select module board
0c02: 79 e8       CALF    $09E8_sendToTx
0c04: 69 8e       MVI     A,$8E
0c06: 79 e8       CALF    $09E8_sendToTx
0c08: 0b          MOV     A,C
0c09: 79 e8       CALF    $09E8_sendToTx
0c0b: b8          RET

adjust switches2
0c0c: 7d 5b       CALF    $0D5B
0c0e: 55 ad 18    OFFIW   $FFAD,$18
0c11: 15 ad 18    ORIW    $FFAD,$18
0c14: 01 b7       LDAW    $FFB7
0c16: 74 98 ad    ORAW    $FFAD
0c19: 63 b7       STAW    $FFB7
0c1b: 16 ff       XRI     A,$FF
0c1d: 74 88 8f    ANAW    $FF8F_switches2
0c20: 1a          MOV     B,A
0c21: 01 a7       LDAW    $FFA7
0c23: 74 88 b7    ANAW    $FFB7
0c26: 60 9a       ORA     A,B
0c28: b8          RET

send switches2 to module board
0c29: 63 8f       STAW    $FF8F_switches2
0c2b: 16 ff       XRI     A,$FF
0c2d: 07 1f       ANI     A,$1F
0c2f: 1b          MOV     C,A
0c30: 69 fd       MVI     A,$FD // select module board // select module board
0c32: 79 e8       CALF    $09E8_sendToTx
0c34: 69 8f       MVI     A,$8F
0c36: 79 e8       CALF    $09E8_sendToTx
0c38: 0b          MOV     A,C
0c39: 79 e8       CALF    $09E8_sendToTx
0c3b: b8          RET

test bits, juggle switches around
0c3c: 59 b6       BIT     1,$FFB6
0c3e: c7          JR      $0C46
0c3f: 01 8e       LDAW    $FF8E_switches1
0c41: 63 b8       STAW    $FFB8
0c43: 05 b6 fd    ANIW    $FFB6,$FD
0c46: b8          RET

manual mode middle bars
0c47: 71 b7 00    MVIW    $FFB7,$00
0c4a: 71 c5 04    MVIW    $FFC5_leftDigit,$04
0c4d: 71 c6 04    MVIW    $FFC6_rightDigit,$04
0c50: 01 c7       LDAW    $FFC7
0c52: 59 b6       BIT     1,$FFB6

switches? falls through from previous
0c54: 01 b8       LDAW    $FFB8
0c56: 63 8e       STAW    $FF8E_switches1
0c58: 7b f9       CALF    $0BF9
0c5a: 01 a7       LDAW    $FFA7
0c5c: 63 8f       STAW    $FF8F_switches2
0c5e: 7c 29       CALF    $0C29
0c60: 6a 00       MVI     B,$00
0c62: 24 58 ff    LXI     DE,$FF58
0c65: 34 6c ff    LXI     HL,$FF6C
0c68: 78 c3       CALF    $08C3
0c6a: b3          PUSH    HL
0c6b: b2          PUSH    DE
0c6c: 6c ff       MVI     D,$FF
0c6e: 34 30 00    LXI     HL,$0030
0c71: ad          LDAX    (HL+B)
0c72: 1d          MOV     E,A
0c73: 0b          MOV     A,C
0c74: 3a          STAX    (DE)
0c75: a2          POP     DE
0c76: a3          POP     HL
0c77: 2a          LDAX    (DE)
0c78: 3b          STAX    (HL)
0c79: 22          INX     DE
0c7a: 32          INX     HL
0c7b: 42          INR     B
0c7c: 74 7a 10    EQI     B,$10
0c7f: e8          JR      $0C68
0c80: 34 90 ff    LXI     HL,$FF90_patchRam
0c83: 6a 0f       MVI     B,$0F
0c85: 2d          LDAX    (HL+)
0c86: 79 e8       CALF    $09E8_sendToTx
0c88: 52          DCR     B
0c89: fb          JR      $0C85
0c8a: 45 a8 30    ONIW    $FFA8_midiSwitch,$30 (MIDI Func1+2)
0c8d: 78 e9       CALF    $08E9
0c8f: 15 b6 02    ORIW    $FFB6,$02
0c92: b8          RET

decimal points off?
0c93: 71 b7 00    MVIW    $FFB7,$00
0c96: 05 c5 ef    ANIW    $FFC5_leftDigit,$EF
0c99: 05 c6 ef    ANIW    $FFC6_rightDigit,$EF
0c9c: b8          RET

shift bits in a and count?
0c9d: 6a 01       MVI     B,$01
0c9f: 48 01       SLRC    A
0ca1: c2          JR      $0CA4
0ca2: 0a          MOV     A,B
0ca3: b8          RET

0ca4: 42          INR     B
0ca5: f9          JR      $0C9F
0ca6: b8          RET

calculate address for patch data
0ca7: 58 b6       BIT     0,$FFB6 // test mode?    // are we in test mode
0ca9: c8          JR      $0CB2      // no, skip ahead
0caa: 6e 0f       MVI     H,$0F      // address is $0f00
0cac: 07 f0       ANI     A,$F0 
0cae: 46 70       ADI     A,$70      
0cb0: 1f          MOV     L,A        // address is now $0f80 to $fd0
0cb1: dd          JR      $0CCF      // return
0cb2: 01 bb       LDAW    $FFBB_patch      // patch digit?
0cb4: 51          DCR     A          // -1
0cb5: 1b          MOV     C,A        // save
0cb6: 01 ba       LDAW    $FFBA_bank      // bank digit
0cb8: 66 10       SUI     A,$10      // -16
0cba: 48 21       SLR     A          // double it
0cbc: 60 9b       ORA     A,C        // OR in patch digit
0cbe: 5e c8       BIT     6,$FFC8_ledCol3    // set for group A
0cc0: 17 40       ORI     A,$40      // not set, group B
0cc2: 44 00 02    LXI     EA,$0200   // form the address in RAM
0cc5: 19          MOV     EAL,A      // address
0cc6: 48 a4       DSLL    EA
0cc8: 48 a4       DSLL    EA
0cca: 48 a4       DSLL    EA
0ccc: 48 a4       DSLL    EA         // multiply by 16 bytes
0cce: b7          DMOV    HL,EA      // HL now looks like $2nn0
0ccf: b8          RET                // return

change patch
called with patch value in C
0cd0: 69 fd       MVI     A,$FD              // select module board // select module board
0cd2: 79 e8       CALF    $09E8_sendToTx
0cd4: 69 90       MVI     A,$90              // start with LFO rate, first slider
0cd6: 79 e8       CALF    $09E8_sendToTx
0cd8: 0b          MOV     A,C
0cd9: 7c a7       CALF    $0CA7_calcPatchAddr    // HL has the address of the patch in RAM
0cdb: 24 90 ff    LXI     DE,$FF90_patchRam  // address to copy to
0cde: 7c f1       CALF    $0CF1              // send 8 bytes
0ce0: 0b          MOV     A,C                // get the switches value
0ce1: 63 8e       STAW    $FF8E_switches1    // save in patchRam
0ce3: 7b f9       CALF    $0BF9              // send switches1 to module
0ce5: 69 98       MVI     A,$98              // VCF LFO depth
0ce7: 79 e8       CALF    $09E8_sendToTx     // start it
0ce9: 7c f1       CALF    $0CF1              // send the next 8 bytes
0ceb: 0b          MOV     A,C                // get switches value
0cec: 63 8f       STAW    $FF8F_switches2    // save in patchRam
0cee: 7c 29       CALF    $0C29              // send switches2 to module
0cf0: b8          RET

transmit patch bytes to module board
decodes bit 7 into C and returns it to send to the switches
0cf1: 6a 07       MVI     B,$07      // seven bits
0cf3: 6b 00       MVI     C,$00      // clear
0cf5: 2d          LDAX    (HL+)      // get byte
0cf6: 48 2a       CLC                // clear carry
0cf8: 57 80       OFFI    A,$80      // if top bit clear
0cfa: 48 2b       STC                // set carry
0cfc: 48 33       RLR     C          // rotate into C
0cfe: 07 7f       ANI     A,$7F      // mask top bit
0d00: 3c          STAX    (DE+)      // store at patchRam
0d01: 79 e8       CALF    $09E8_sendToTx
0d03: 52          DCR     B          // count down
0d04: f0          JR      $0CF5      // loop back around for next byte
0d05: b8          RET

something to do with patches, possibly packing?
0d06: 0b          MOV     A,C
0d07: 63 b9       STAW    $FFB9
0d09: 7c a7       CALF    $0CA7_calcPatchAddr
0d0b: 24 90 ff    LXI     DE,$FF90_patchRam
0d0e: 01 8e       LDAW    $FF8E_switches1
0d10: 1b          MOV     C,A
0d11: 7d 25       CALF    $0D25
0d13: 01 8f       LDAW    $FF8F_switches2
0d15: 1b          MOV     C,A
0d16: 7d 25       CALF    $0D25
0d18: 15 b6 04    ORIW    $FFB6,$04
0d1b: 7c 3c       CALF    $0C3C
0d1d: 01 b9       LDAW    $FFB9
0d1f: 1b          MOV     C,A
0d20: 7c d0       CALF    $0CD0
0d22: 7c 93       CALF    $0C93
0d24: b8          RET

encode switches byte into upper bits?
0d25: 6a 07       MVI     B,$07
0d27: 2c          LDAX    (DE+)
0d28: 07 7f       ANI     A,$7F
0d2a: 48 03       SLRC    C
0d2c: c2          JR      $0D2F
0d2d: 17 80       ORI     A,$80
0d2f: 3d          STAX    (HL+)
0d30: 52          DCR     B
0d31: f5          JR      $0D27
0d32: b8          RET

save digit pattern
0d33: 01 c5       LDAW    $FFC5_leftDigit
0d35: 63 c9       STAW    $FFC9
0d37: 01 c6       LDAW    $FFC6_rightDigit
0d39: 63 ca       STAW    $FFCA
0d3b: b8          RET     

restore digit pattern
0d3c: 01 c9       LDAW    $FFC9
0d3e: 63 c5       STAW    $FFC5_leftDigit
0d40: 01 ca       LDAW    $FFCA
0d42: 63 c6       STAW    $FFC6_rightDigit
0d44: b8          RET

look up digit in table
called with value in A to show on LED
0d45: 34 50 00    LXI     HL,$0050   // bitmap
0d48: 1b          MOV     C,A        // save
0d49: 48 21       SLR     A
0d4b: 48 21       SLR     A
0d4d: 48 21       SLR     A
0d4f: 48 21       SLR     A          // upper nybble
0d51: ac          LDAX    (HL+A)     // fetch bit pattern from table
0d52: 63 c5       STAW    $FFC5_leftDigit      // store
0d54: 0b          MOV     A,C
0d55: 07 0f       ANI     A,$0F      // lower nybble
0d57: ac          LDAX    (HL+A)     // fetch bit pattern from table
0d58: 63 c6       STAW    $FFC6_rightDigit      // store
0d5a: b8          RET

switch on decimal points
0d5b: 15 c5 10    ORIW    $FFC5_leftDigit_leftDigit,$10
0d5e: 15 c6 10    ORIW    $FFC6_rightDigit,$10
0d61: b8          RET

probably something to do with tape
0d62: 07 38       ANI     A,$38              // mask off all but LOAD/SAVE/VERIFY
0d64: 5f c8       BIT     7,$FFC8_ledCol3    // Group B
0d66: 17 04       ORI     A,$04              // 
0d68: 63 d1       STAW    $FFD1              // save somewhere
0d6a: 64 05 ff    MVI     PF,$FF             // column 3 LEDs
0d6d: 4d c1       MOV     PB,A               // enable
0d6f: 24 ff 1f    LXI     DE,$1FFF
0d72: 4a df       MVIX    DE,$DF
0d74: 7b 70       CALF    $0B70_stopAllNotes //
0d76: 69 87       MVI     A,$87              // and also sustain off
0d78: 79 e8       CALF    $09E8_sendToTx     //
0d7a: 6a ff       MVI     B,$FF
0d7c: 48 2d       MUL     A
0d7e: 52          DCR     B
0d7f: fc          JR      $0D7C              // wait
0d80: ba          DI      
0d81: 64 01 00    MVI     PB,$00
0d84: 64 05 c0    MVI     PF,$C0
0d87: 05 d1 3c    ANIW    $FFD1,$3C
0d8a: 55 d1 20    OFFIW   $FFD1,$20
0d8d: 4e 69       JRE     $0DF8
0d8f: 55 d1 10    OFFIW   $FFD1,$10
0d92: 4e 6d       JRE     $0E01
0d94: 45 d1 08    ONIW    $FFD1,$08
0d97: 4e f7       JRE     $0E90
0d99: 64 85 bf    MVI     TMM,$BF
0d9c: 48 42       SKIT    FT1
0d9e: 00          NOP     
0d9f: 64 01 08    MVI     PB,$08
0da2: 44 00 24    LXI     EA,$2400
0da5: 5a d1       BIT     2,$FFD1
0da7: 44 00 28    LXI     EA,$2800
0daa: 34 00 20    LXI     HL,$2000
0dad: 5a d1       BIT     2,$FFD1
0daf: 34 00 24    LXI     HL,$2400
0db2: 24 c0 00    LXI     DE,$00C0
0db5: 15 d1 01    ORIW    $FFD1,$01
0db8: 69 01       MVI     A,$01
0dba: 4d db       MOV     TM1,A
0dbc: 64 8d 7f    ANI     TMM,$7F
0dbf: 7e b4       CALF    $0EB4
0dc1: 69 4a       MVI     A,$4A
0dc3: 1a          MOV     B,A
0dc4: 60 44       ADD     D,A
0dc6: 7f 07       CALF    $0F07
0dc8: 7e f0       CALF    $0EF0
0dca: 69 55       MVI     A,$55
0dcc: 1a          MOV     B,A
0dcd: 60 44       ADD     D,A
0dcf: 7f 07       CALF    $0F07
0dd1: 7e f0       CALF    $0EF0
0dd3: 7f 01       CALF    $0F01
0dd5: 2d          LDAX    (HL+)
0dd6: 1a          MOV     B,A
0dd7: 60 44       ADD     D,A
0dd9: 7f 07       CALF    $0F07
0ddb: 7e f0       CALF    $0EF0
0ddd: 74 ff       DEQ     EA,HL
0ddf: f3          JR      $0DD3
0de0: 0c          MOV     A,D
0de1: 1a          MOV     B,A
0de2: 7f 07       CALF    $0F07
0de4: 6a aa       MVI     B,$AA
0de6: 7f 07       CALF    $0F07
0de8: 6a aa       MVI     B,$AA
0dea: 7f 07       CALF    $0F07
0dec: 5e d1       BIT     6,$FFD1
0dee: c4          JR      $0DF3
0def: 7e b4       CALF    $0EB4
0df1: 4e 9d       JRE     $0E90
0df3: 15 d1 40    ORIW    $FFD1,$40
0df6: 4f a1       JRE     $0D99
0df8: 64 4a 10    ONI     PC,$10
0dfb: 4e 9f       JRE     $0E9C
0dfd: 64 01 20    MVI     PB,$20
0e00: c3          JR      $0E04
0e01: 64 01 10    MVI     PB,$10
0e04: 5a d1       BIT     2,$FFD1
0e06: ca          JR      $0E11
0e07: 64 11 40    XRI     PB,$40
0e0a: 34 00 20    LXI     HL,$2000
0e0d: 44 00 24    LXI     EA,$2400
0e10: c9          JR      $0E1A
0e11: 64 11 80    XRI     PB,$80
0e14: 34 00 24    LXI     HL,$2400
0e17: 44 00 28    LXI     EA,$2800
0e1a: 64 85 b7    MVI     TMM,$B7
0e1d: 69 0d       MVI     A,$0D
0e1f: 4d da       MOV     TM0,A
0e21: 69 1e       MVI     A,$1E
0e23: 4d db       MOV     TM1,A
0e25: 15 d1 01    ORIW    $FFD1,$01
0e28: 64 4a 20    ONI     PC,$20
0e2b: 05 d1 fe    ANIW    $FFD1,$FE
0e2e: 48 41       SKIT    FT0
0e30: 00          NOP     
0e31: 48 42       SKIT    FT1
0e33: 00          NOP     
0e34: 6b ff       MVI     C,$FF
0e36: 64 8d 6f    ANI     TMM,$6F
0e39: 7f 38       CALF    $0F38
0e3b: 48 0a       SK      CY
0e3d: f6          JR      $0E34
0e3e: 7e f0       CALF    $0EF0
0e40: 53          DCR     C
0e41: f7          JR      $0E39
0e42: 15 d1 02    ORIW    $FFD1,$02
0e45: 14 00 80    LXI     BC,$8000
0e48: 7f 38       CALF    $0F38
0e4a: 48 1a       SKN     CY
0e4c: 4e 30       JRE     $0E7E
0e4e: 6b 00       MVI     C,$00
0e50: 7f 27       CALF    $0F27
0e52: 77 4a       EQI     A,$4A
0e54: d3          JR      $0E68
0e55: 60 43       ADD     C,A
0e57: 7f 18       CALF    $0F18
0e59: 77 55       EQI     A,$55
0e5b: cc          JR      $0E68
0e5c: 60 43       ADD     C,A
0e5e: 7f 18       CALF    $0F18
0e60: 60 43       ADD     C,A
0e62: 45 d1 10    ONIW    $FFD1,$10
0e65: c5          JR      $0E6B
0e66: 70 fd       EQAX    (HL+)
0e68: 4e 22       JRE     $0E8C
0e6a: c5          JR      $0E70
0e6b: 64 4a 10    ONI     PC,$10
0e6e: dd          JR      $0E8C
0e6f: 3d          STAX    (HL+)
0e70: 74 ff       DEQ     EA,HL
0e72: eb          JR      $0E5E
0e73: 7f 18       CALF    $0F18
0e75: 60 fb       EQA     A,C
0e77: d4          JR      $0E8C
0e78: 7f 18       CALF    $0F18
0e7a: 77 aa       EQI     A,$AA
0e7c: cf          JR      $0E8C
0e7d: d2          JR      $0E90
0e7e: 5c d1       BIT     4,$FFD1
0e80: 7e fb       CALF    $0EFB
0e82: 7e f0       CALF    $0EF0
0e84: 13          DCX     BC
0e85: 0b          MOV     A,C
0e86: 60 9a       ORA     A,B
0e88: 77 00       EQI     A,$00
0e8a: 4f bc       JRE     $0E48
0e8c: 15 d1 80    ORIW    $FFD1,$80
0e8f: c3          JR      $0E93
0e90: 05 d1 7f    ANIW    $FFD1,$7F
0e93: 64 01 00    MVI     PB,$00
0e96: 64 85 bf    MVI     TMM,$BF
0e99: 5f d1       BIT     7,$FFD1
0e9b: d1          JR      $0EAD
0e9c: 69 ce       MVI     A,$CE
0e9e: 5f d1       BIT     7,$FFD1
0ea0: 69 de       MVI     A,$DE
0ea2: 7d 45       CALF    $0D45_digitLookup
0ea4: 04 ff ff    LXI     SP,$FFFF
0ea7: 64 80 08    MVI     ANM,$08
0eaa: 54 79 00    JMP     $0079
0ead: 71 c5 04    MVIW    $FFC5_leftDigit,$04
0eb0: 71 c6 04    MVIW    $FFC6_rightDigit,$04
0eb3: f0          JR      $0EA4

uknown2
0eb4: 6a 18       MVI     B,$18
0eb6: 6b ff       MVI     C,$FF
0eb8: 7e d5       CALF    $0ED5
0eba: 7e f0       CALF    $0EF0
0ebc: 53          DCR     C
0ebd: fa          JR      $0EB8
0ebe: 7f 01       CALF    $0F01
0ec0: 52          DCR     B
0ec1: f4          JR      $0EB6
0ec2: b8          RET   


0ec3: 0d          MOV     A,E
0ec4: 58 d1       BIT     0,$FFD1
0ec6: c8          JR      $0ECF
0ec7: 16 c0       XRI     A,$C0
0ec9: 1d          MOV     E,A
0eca: 69 05       MVI     A,$05
0ecc: 7e e7       CALF    $0EE7
0ece: b8          RET


0ecf: 15 d1 01    ORIW    $FFD1,$01
0ed2: 16 80       XRI     A,$80
0ed4: f4          JR      $0EC9

unknown5, probably still tape
0ed5: 0d          MOV     A,E
0ed6: 58 d1       BIT     0,$FFD1
0ed8: cb          JR      $0EE4
0ed9: 05 d1 fe    ANIW    $FFD1,$FE
0edc: 16 80       XRI     A,$80
0ede: 1d          MOV     E,A
0edf: 69 14       MVI     A,$14
0ee1: 7e e7       CALF    $0EE7
0ee3: b8          RET     
0ee4: 16 c0       XRI     A,$C0
0ee6: f7          JR      $0EDE

unknown3
0ee7: 48 42       SKIT    FT1
0ee9: fd          JR      $0EE7
0eea: 4d db       MOV     TM1,A
0eec: 0d          MOV     A,E
0eed: 4d c2       MOV     PC,A
0eef: b8          RET

called from seven places
possibly to do with tape
0ef0: 5a d1       BIT     2,$FFD1    // status bit?
0ef2: c4          JR      $0EF7
0ef3: 64 11 40    XRI     PB,$40     // toggle a scan line for LEDs?
0ef6: b8          RET     
0ef7: 64 11 80    XRI     PB,$80     // toggle a scan line for LEDs?
0efa: b8          RET

memory protect switch
0efb: 64 4a 10    ONI     PC,$10
0efe: 4f 8c       JRE     $0E8C
0f00: b8          RET

two keys pressed together, maybe poly1+2?
0f01: 64 48 06    ONI     PA,$06
0f04: b8          RET
0f05: 4f 89       JRE     $0E90

possibly to do with tape
0f07: 6b 07       MVI     C,$07
0f09: 7e c3       CALF    $0EC3
0f0b: 48 02       SLRC    B
0f0d: c3          JR      $0F11
0f0e: 7e d5       CALF    $0ED5
0f10: c2          JR      $0F13
0f11: 7e c3       CALF    $0EC3
0f13: 53          DCR     C
0f14: f6          JR      $0F0B
0f15: 7e d5       CALF    $0ED5
0f17: b8          RET

unknown6
0f18: 5a d1       BIT     2,$FFD1
0f1a: c4          JR      $0F1F
0f1b: 64 11 40    XRI     PB,$40
0f1e: c3          JR      $0F22
0f1f: 64 11 80    XRI     PB,$80
0f22: 7f 38       CALF    $0F38
0f24: 48 1a       SKN     CY
0f26: ce          JR      $0F35


0f27: 69 00       MVI     A,$00
0f29: 6a 07       MVI     B,$07
0f2b: 7f 38       CALF    $0F38
0f2d: 48 31       RLR     A
0f2f: 52          DCR     B
0f30: fa          JR      $0F2B
0f31: 7f 38       CALF    $0F38
0f33: 48 0a       SK      CY
0f35: 4f 55       JRE     $0E8C
0f37: b8          RET

unknown7
0f38: 7f 01       CALF    $0F01
0f3a: 58 d1       BIT     0,$FFD1
0f3c: cc          JR      $0F49
0f3d: 05 d1 fe    ANIW    $FFD1,$FE
0f40: 64 4a 20    ONI     PC,$20
0f43: d1          JR      $0F55
0f44: 48 62       SKNIT   FT1
0f46: 7f 64       CALF    $0F64
0f48: f7          JR      $0F40
0f49: 15 d1 01    ORIW    $FFD1,$01
0f4c: 64 5a 20    OFFI    PC,$20
0f4f: c5          JR      $0F55
0f50: 48 62       SKNIT   FT1
0f52: 7f 64       CALF    $0F64
0f54: f7          JR      $0F4C
0f55: 48 41       SKIT    FT0
0f57: c9          JR      $0F61
0f58: 48 2b       STC     
0f5a: 64 95 90    XRI     TMM,$90
0f5d: 64 95 90    XRI     TMM,$90
0f60: b8          RET     
0f61: 48 2a       CLC     
0f63: f6          JR      $0F5A

unknown4
0f64: 55 d1 02    OFFIW   $FFD1,$02
0f67: 4f 23       JRE     $0E8C
0f69: 64 95 90    XRI     TMM,$90
0f6c: 64 95 90    XRI     TMM,$90
0f6f: 48 41       SKIT    FT0
0f71: 00          NOP     
0f72: 6b ff       MVI     C,$FF
0f74: 5c d1       BIT     4,$FFD1
0f76: 7e fb       CALF    $0EFB
0f78: 7f 01       CALF    $0F01
0f7a: 7e f0       CALF    $0EF0
0f7c: b8          RET     
0f7d: 00          NOP     
0f7e: 00          NOP     
0f7f: 00          NOP     
0f80: 40 80 00    CALL    $0080
0f83: 00          NOP     
0f84: 00          NOP     
0f85: ff          JR      $0F85
0f86: 00          NOP     
0f87: 00          NOP     
0f88: 00          NOP     
0f89: 7f 40       CALF    $0F40
0f8b: 00          NOP     
0f8c: 80          CALT    ($0080)
0f8d: 00          NOP     
0f8e: 00          NOP     
0f8f: 00          NOP     
0f90: 40 80 00    CALL    $0080
0f93: 00          NOP     
0f94: 00          NOP     
0f95: ff          JR      $0F95
0f96: 00          NOP     
0f97: 00          NOP     
0f98: 00          NOP     
0f99: 7f 40       CALF    $0F40
0f9b: 00          NOP     
0f9c: 80          CALT    ($0080)
0f9d: 7f 00       CALF    $0F00
0f9f: 7f 40       CALF    $0F40
0fa1: 80          CALT    ($0080)
0fa2: 00          NOP     
0fa3: 00          NOP     
0fa4: 00          NOP     
0fa5: b1          PUSH    BC
0fa6: 7f 00       CALF    $0F00
0fa8: 00          NOP     
0fa9: 7f 40       CALF    $0F40
0fab: 00          NOP     
0fac: 80          CALT    ($0080)
0fad: 7f 00       CALF    $0F00
0faf: 00          NOP     
0fb0: 40 80 00    CALL    $0080
0fb3: 00          NOP     
0fb4: 80          CALT    ($0080)
0fb5: ff          JR      $0FB5
0fb6: 00          NOP     
0fb7: 00          NOP     
0fb8: 00          NOP     
0fb9: 7f 40       CALF    $0F40
0fbb: 00          NOP     
0fbc: 80          CALT    ($0080)
0fbd: 7f 00       CALF    $0F00
0fbf: 00          NOP     
0fc0: 40 80 00    CALL    $0080
0fc3: 80          CALT    ($0080)
0fc4: 00          NOP     
0fc5: ff          JR      $0FC5
0fc6: 00          NOP     
0fc7: 00          NOP     
0fc8: 80          CALT    ($0080)
0fc9: 7f 40       CALF    $0F40
0fcb: 00          NOP     
0fcc: 80          CALT    ($0080)
0fcd: 7f 00       CALF    $0F00
0fcf: 00          NOP     
0fd0: 40 80 00    CALL    $0080
0fd3: 00          NOP     
0fd4: 7f ff       CALF    $0FFF
0fd6: 00          NOP     
0fd7: 00          NOP     
0fd8: 00          NOP     
0fd9: 7f 40       CALF    $0F40
0fdb: 00          NOP     
0fdc: 80          CALT    ($0080)
0fdd: 7f 00       CALF    $0F00
0fdf: 00          NOP     
0fe0: 40 80 00    CALL    $0080
0fe3: 00          NOP     
0fe4: 00          NOP     
0fe5: ff          JR      $0FE5
0fe6: 7f 00       CALF    $0F00
0fe8: 00          NOP     
0fe9: 7f 40       CALF    $0F40
0feb: 00          NOP     
0fec: 80          CALT    ($0080)
0fed: 7f 00       CALF    $0F00
0fef: 00          NOP     
0ff0: 40 80 00    CALL    $0080
0ff3: 80          CALT    ($0080)
0ff4: 00          NOP     
0ff5: ff          JR      $0FF5
0ff6: 00          NOP     
0ff7: 00          NOP     
0ff8: 00          NOP     
0ff9: 7f 40       CALF    $0F40
0ffb: 00          NOP     
0ffc: 8a          CALT    ($0094)
0ffd: 00          NOP     
0ffe: 0a          MOV     A,B
0fff: 00          NOP     
