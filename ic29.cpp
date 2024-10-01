isassembly of voice.bin, 4096 bytes [0x0 to 0x0fff]
0000: 54 80 02    JMP     $0280
0003: 00          NOP   

NMI ISR
0004: aa          EI      
0005: 62          RETI    
0006: 00          NOP     
0007: 00          NOP     
//
bitmask lookup table
0008: fe fd fb f7 ef df

External INT1/2 ISR
0010: aa          EI      
0011: 62          RETI 

bitmask lookup table
0012: 01 02 04 08 10 20


Timer/Counter INTE0/1 ISR
0018: aa          EI
0019: 62          RETI    
001a: 00          NOP     
001b: 00          NOP     
001c: 00          NOP     
001d: 00          NOP     
001e: 00          NOP     
001f: 00          NOP

Timer/Counter/ADC ISR
0020: 4e 4e       JRE     $0070
0022: 00          NOP     
0023: 00          NOP     
0024: 00          NOP     
0025: 00          NOP     
0026: 00          NOP     
0027: 00          NOP

Serial ISR
0028: 4e 64       JRE     $008E
002a: 00          NOP     
002b: 00          NOP     
002c: 00          NOP     
002d: 00          NOP     
002e: 00          NOP     
002f: 00          NOP     
0030: 00          NOP     
0031: 00          NOP     
0032: 00          NOP     
0033: 00          NOP     
0034: 00          NOP     
0035: 00          NOP     
0036: 00          NOP     
0037: 00          NOP     
0038: 00          NOP     
0039: 00          NOP     
003a: 00          NOP     
003b: 00          NOP     
003c: 00          NOP     
003d: 00          NOP     
003e: 00          NOP     
003f: 00          NOP     
0040: 00          NOP     
0041: 00          NOP     
0042: 00          NOP     
0043: 00          NOP     
0044: 00          NOP     
0045: 00          NOP     
0046: 00          NOP     
0047: 00          NOP     
0048: 00          NOP     
0049: 00          NOP     
004a: 00          NOP     
004b: 00          NOP     
004c: 00          NOP     
004d: 00          NOP     
004e: 00          NOP     
004f: 00          NOP     
0050: 00          NOP     
0051: 00          NOP     
0052: 00          NOP     
0053: 00          NOP     
0054: 00          NOP     
0055: 00          NOP     
0056: 00          NOP     
0057: 00          NOP     
0058: 00          NOP     
0059: 00          NOP     
005a: 00          NOP     
005b: 00          NOP     
005c: 00          NOP     
005d: 00          NOP     
005e: 00          NOP     
005f: 00          NOP

SWI ISR
0060: aa          EI      
0061: 62          RETI    
0062: 00          NOP     
0063: 00          NOP     
0064: 00          NOP     
0065: 00          NOP     
0066: 00          NOP     
0067: 00          NOP     
0068: 00          NOP     
0069: 00          NOP     
006a: 00          NOP     
006b: 00          NOP     
006c: 00          NOP     
006d: 00          NOP     
006e: 00          NOP     
006f: 00          NOP

AD Converter ISR
0070: 10          EXA     
0071: 11          EXX                // alternate registers
0072: 4c c8       MOV     A,ANM      // mode register
0074: 64 90 08    XRI     ANM,$08    // toggle between inputs 0-3 and 4-7
0077: 34 5c ff    LXI     HL,$FF5C_adcMode   
007a: 3d          STAX    (HL+)      // save ADC mode
007b: 4c e0       MOV     A,CR0      
007d: 3d          STAX    (HL+)      // channel 0/4
007e: 4c e1       MOV     A,CR1
0080: 3d          STAX    (HL+)      // channel 1/5
0081: 4c e2       MOV     A,CR2
0083: 3d          STAX    (HL+)      // channel 2/6
0084: 4c e3       MOV     A,CR3
0086: 3b          STAX    (HL)       // channel 3/7
0087: 64 06 05    MVI     MKH,$05    //reset interrupt
008a: 10          EXA                // swap back
008b: 11          EXX     
008c: aa          EI                 // enable interrupts and return
008d: 62          RETI

serial interrupt handler
008e: 10          EXA     
008f: 11          EXX     
0090: 4c d9       MOV     A,RXB              // fetch the received byte
0092: 47 80       ONI     A,$80              // is it a command byte
0094: 4e 2e       JRE     $00C4              // yes
0096: 63 36       STAW    $FF36_cmdByte      // store it as the current command byte
0098: 37 88       LTI     A,$88              // less than $88 (voice off, sustain on/off message)
009a: 4e 6d       JRE     $0109_retSerialIsr              // no
009c: 37 86       LTI     A,$86              // less than $86 (voice off)
009e: d8          JR      $00B7              // no
009f: 66 76       SUI     A,$76              // get voice number
00a1: 1f          MOV     L,A                
00a2: 6e 00       MVI     H,$00              // hl now points to the inverted mask table
00a4: 2b          LDAX    (HL)               // fetch mask in A
00a5: 74 88 10    ANAW    $FF10_noteGate     // AND it with the note flags
00a8: 63 10       STAW    $FF10_noteGate     // save it back
00aa: 55 1e 01    OFFIW   $FF1E_flags1,$01   // sustain?
00ad: 4e 9a       JRE     $0149
00af: 2b          LDAX    (HL)               // do the same to these flags
00b0: 74 88 33    ANAW    $FF33_rlsPhase
00b3: 63 33       STAW    $FF33_rlsPhase
00b5: 4e 92       JRE     $0149

handle a sustain message
00b7: 77 86       EQI     A,$86              // sustain on
00b9: c5          JR      $00BF              // no, do sustain off
00ba: 15 1e 01    ORIW    $FF1E_flags1,$01   // yes, set flag
00bd: 4e 4a       JRE     $0109_retSerialIsr5
00bf: 05 1e fe    ANIW    $FF1E_flags1,$FE   // mask off sustain bit
00c2: 4e 45       JRE     $0109_retSerialIsr

handle a value byte
00c4: 1c          MOV     D,A
00c5: 01 36       LDAW    $FF36_cmdByte      // fetch command byte
00c7: 27 87       GTI     A,$87              // voice off and sustain messages don't take a parameter so return
00c9: 4e 3e       JRE     $0109_retSerialIsr
00cb: 37 a4       LTI     A,$A4              // no commands above $A4 so return
00cd: 4e 3a       JRE     $0109_retSerialIsr
00cf: 27 8d       GTI     A,$8D              // $88-$8d are voice on, greater?
00d1: 4e 3a       JRE     $010d_voiceOn              // no, handle voice on
00d3: 66 8e       SUI     A,$8E              // yes, get the control value
00d5: 48 25       SLL     A                  // double it
okay the next two instructions are fun
TABLE will return the value at PC + A + 1 in BC
JB will then jump to BC
00d7: 48 a8       TABLE   
00d9: 21          JB   
  
this table then holds the addresses of the routines to handle incoming parameter values
these are in the same order they appear in sysex, which makes life easier
00da: 014f 0184 0198
00e0: 0200 023a 01ab 01c8 01cc 01d0 01af 01b3
00f0: 01b7 01d4 01e2 01ed 01c4 01e9 01c0 0223
0100: 0227 01a7 0246 

return from interrupt after handling value
this increments the command byte implementing a kind of "running status"
so we can just blast patch values at the voice CPU and have it update them all
or send an individual single value
0106: 20 36       INRW    $FF36_cmdByte
0108: 00          NOP                        // INRW might skip
0109: 10          EXA     
010a: 11          EXX     
010b: aa          EI      
010c: 62          RETI    

voice on commands
command in A, note value in D
010d: 1a          MOV     B,A                // save command
010e: 66 76       SUI     A,$76              // form offset to bitmask table
0110: 1f          MOV     L,A
0111: 6e 00       MVI     H,$00              
0113: 2b          LDAX    (HL)               // bit for voice
0114: 1d          MOV     E,A                // save
0115: 74 98 10    ORAW    $FF10_noteGate     // OR it into voice gate table
0118: 63 10       STAW    $FF10_noteGate     
011a: 0d          MOV     A,E                // fetch the bit again
011b: 74 98 07    ORAW    $FF07_atkPhase     // enable attack phase
011e: 63 07       STAW    $FF07_atkPhase 
0120: 0a          MOV     A,B                // get the command back
0121: 66 88       SUI     A,$88              // work out the voice number
0123: 1a          MOV     B,A                // to get the existing note value
0124: 34 09 ff    LXI     HL,$FF09_voice1Note    
0127: ad          LDAX    (HL+B)
0128: 60 ec       NEA     A,D                // is it the same note number
012a: d9          JR      $0144              // yes
012b: 0c          MOV     A,D                // no
012c: bd          STAX    (HL+B)             // store it in the voice note table
012d: 0d          MOV     A,E                // fetch the bit again
012e: 74 c8 11    ONAW    $FF11_voiceRun     // is the voice already running
0131: cc          JR      $013E              // no, reset the 8253 channel
0132: 74 c8 33    ONAW    $FF33_rlsPhase     // yes, is the voice in release
0135: d3          JR      $0149              // no
0136: 16 ff       XRI     A,$FF              // invert the bit
0138: 74 88 33    ANAW    $FF33_rlsPhase     // mask off release flag
013b: 63 33       STAW    $FF33_rlsPhase
013d: cb          JR      $0149              
013e: 74 98 00    ORAW    $FF00_resetVoiceBits
0141: 63 00       STAW    $FF00_resetVoiceBits
0143: c5          JR      $0149
same note
0144: 0d          MOV     A,E                // get voice bit back
0145: 74 d8 11    OFFAW   $FF11_voiceRun     // is the voice running?
0148: e9          JR      $0132              // no, jump back and check for release
clobber stack return
0149: 04 ff ff    LXI     SP,$FFFF           // clobber the stack 
014c: 54 eb 02    JMP     $02EB              // jump ahead

handle voice parameters
//
switch 1 values
014f: 0c          MOV     A,D                // save value
0150: 63 7f       STAW    $FF7F_switch1      // save raw value in RAM
0152: 07 10       ANI     A,$10              // saw off/on
0154: 1a          MOV     B,A
0155: 01 46       LDAW    $FF46_ic40Latch    // fetch stored value for latch
0157: 07 0c       ANI     A,$0C              // mask off HPF bits
0159: 60 9a       ORA     A,B                // OR in saw on/off
015b: 5d 7f       BIT     5,$FF7F_switch1    // chorus on/off
015d: 17 01       ORI     A,$01              // bit 0 turns on chorus
015f: 74 5c 40    OFFI    D,$40              // chorus fast/slow
0162: 17 02       ORI     A,$02              // OR it in
0164: 63 46       STAW    $FF46_ic40Latch    // store this to be latched into IC40
0166: 15 1e 40    ORIW    $FF1E_flags1,$40   // sqr on
0169: 74 5c 08    OFFI    D,$08              // is it off in the command
016c: 05 1e bf    ANIW    $FF1E_flags1,$BF   // turn sqr off
016f: 74 5c 04    OFFI    D,$04              // convert lower three bits to divider value
0172: c4          JR      $0177
0173: 64 05 c0    MVI     PF,$C0             // upper two bits of PF set ratio
0176: cb          JR      $0182
0177: 74 5c 02    OFFI    D,$02
017a: c4          JR      $017F
017b: 64 05 40    MVI     PF,$40
017e: c3          JR      $0182
017f: 64 05 00    MVI     PF,$00
0182: 4f 82       JRE     $0106_retSerialValue

switch 2 values
only the HPF bits need special consideration
0184: 0c          MOV     A,D
0185: 63 37       STAW    $FF37_switch2      // save value
0187: 16 18       XRI     A,$18              // invert the HPF bits
0189: 07 18       ANI     A,$18              // get only the HPF bits
018b: 48 21       SLR     A                  // shift right  ($18 >> 1 = $0c)
018d: 1a          MOV     B,A                
018e: 01 46       LDAW    $FF46_ic40Latch    // fetch stored value for latch
0190: 07 f3       ANI     A,$F3              // mask out HPF bits
0192: 60 9a       ORA     A,B                // OR them in
0194: 63 46       STAW    $FF46_ic40Latch    // save it back
0196: 4f 6e       JRE     $0106_retSerialValue

LFO rate
0198: 0c          MOV     A,D
0199: 48 25       SLL     A                  // 2-byte values
019b: 34 60 0c    LXI     HL,$0C60_lfoSpeedTbl
019e: 24 4b ff    LXI     DE,$FF4B_lfoRateCoeff
01a1: 48 8c       LDEAX   (HL+A)             // look up LFO coefficient
01a3: 48 92       STEAX   (DE)               // save
01a5: 4f 5f       JRE     $0106_retSerialValue

these values are simply doubled to go from 0-127 to 0-254
and then saved as single bytes
01a7: 34 63 ff    LXI     HL,$FF63_midiModDepth
01aa: cf          JR      $01BA_storeDoubled              // store
01ab: 34 47 ff    LXI     HL,$FF47_pwmDepth
01ae: cb          JR      $01BA_storeDoubled              // store
01af: 34 41 ff    LXI     HL,$FF41_vcfEnvMod
01b2: c7          JR      $01BA_storeDoubled          
01b3: 34 48 ff    LXI     HL,$FF48_lfoToVcf
01b6: c3          JR      $01BA_storeDoubled
01b7: 34 42 ff    LXI     HL,$FF42_vcfKeyTrack

01BA_storeDoubled: 0c          MOV     A,D
01bb: 48 25       SLL     A
01bd: 3b          STAX    (HL)
01be: 4f 46       JRE     $0106_retSerialValue

these values are converted to a 16-bit value
they change from 0-127 to 0-$3f80
01c0: 34 3b ff    LXI     HL,$FF3B_subOscLev
01c3: d3          JR      $01D7_store16Bit
01c4: 34 23 ff    LXI     HL,$FF23_envSustainLev
01c7: cf          JR      $01D7_store16Bit
01c8: 34 39 ff    LXI     HL,$FF39_noiseLevel
01cb: cb          JR      $01D7_store16Bit
01cc: 34 3d ff    LXI     HL,$FF3D_vcfCutoff
01cf: c7          JR      $01D7_store16Bit
01d0: 34 3f ff    LXI     HL,$FF3F_vcfReso
01d3: c3          JR      $01D7_store16Bit
01d4: 34 43 ff    LXI     HL,$FF43_vcaMaster_vcaMaster
01D7_store16Bit: 44 00 00    LXI     EA,$0000
01da: 0c          MOV     A,D
01db: 18          MOV     EAH,A
01dc: 48 a0       DSLR    EA
01de: 48 93       STEAX   (HL)
01e0: 4f 24       JRE     $0106_retSerialValue

attack is just saved as an 8-bit value
it is looked up later
01e2: 34 45 ff    LXI     HL,$FF45_envAtkVal
01e5: 0c          MOV     A,D
01e6: 3b          STAX    (HL)
01e7: 4f 1d       JRE     $0106_retSerialValue

release and decay are looked up in a table
01e9: 34 25 ff    LXI     HL,$FF25_envRlsCoeff
01ec: c3          JR      $01F0
01ed: 34 21 ff    LXI     HL,$FF21_envDcyCoeff
01f0: 0c          MOV     A,D
01f1: 48 25       SLL     A
01f3: 44 60 0d    LXI     EA,$0D60_envDecRelTbl
01f6: 70 41       EADD    EA,A
01f8: b6          DMOV    DE,EA
01f9: 48 82       LDEAX   (DE)
01fb: 48 93       STEAX   (HL)
01fd: 54 06 01    JMP     $0106_retSerialValue

LFO Delay requires a bit of special attention
0200: 0c          MOV     A,D
0201: 48 21       SLR     A
0203: 48 21       SLR     A
0205: 48 21       SLR     A
0207: 07 0e       ANI     A,$0E
0209: 34 30 0b    LXI     HL,$0B30_lfoDelayTbl1
020c: 48 8c       LDEAX   (HL+A)
020e: 34 6c ff    LXI     HL,$FF6C_lfoHoldoffCoeff
0211: 48 93       STEAX   (HL)
0213: 0c          MOV     A,D
0214: 48 25       SLL     A
0216: 34 60 0b    LXI     HL,$0B60_envAtkTbl
0219: 48 8c       LDEAX   (HL+A)
021b: 24 58 ff    LXI     DE,$FF58_lfoDelayCoeff
021e: 48 92       STEAX   (DE)
0220: 54 06 01    JMP     $0106_retSerialValue

negative bend value
0223: 05 1e ef    ANIW    $FF1E_flags1,$EF // bend negative
0226: c3          JR      $022A
positive bend value
0227: 15 1e 10    ORIW    $FF1E_flags1,$10 // bend positive
022a: 0c          MOV     A,D
022b: 67 00       NEI     A,$00          // bend is zero?
022d: c4          JR      $0232          // yes
022e: 48 2b       STC           
0230: 48 35       RLL     A              // double it and add 1
0232: 63 62       STAW    $FF62_midiBendAmt
0234: 71 05 01    MVIW    $FF05_bendPolarity,$01
0237: 54 06 01    JMP     $0106_retSerialValue

DCO LFO depth is looked up in a table of 8-bit values
023a: 0c          MOV     A,D
023b: 34 80 0a    LXI     HL,$0A80_lfoDepthTbl
023e: 24 49 ff    LXI     DE,$FF49_lfoToPitchScaler
0241: ac          LDAX    (HL+A)
0242: 3a          STAX    (DE)
0243: 54 06 01    JMP     $0106_retSerialValue

Set DAC to Zero command for test mode
0246: 5e 37       BIT     6,$FF37_switch2
0248: 54 09 01    JMP     $0109_retSerialIsr     // test mode not armed
024b: 0c          MOV     A,D
024c: 63 01       STAW    $FF01_testMode                  // set test mode flag
024e: 10          EXA     
024f: 11          EXX     
0250: aa          EI      
0251: 54 b8 07    JMP     $07B8

0254: 00          NOP     
0255: 00          NOP     
0256: 00          NOP     
0257: 00          NOP     
0258: 00          NOP     
0259: 00          NOP     
025a: 00          NOP     
025b: 00          NOP     
025c: 00          NOP     
025d: 00          NOP     
025e: 00          NOP     
025f: 00          NOP     
0260: 00          NOP     
0261: 00          NOP     
0262: 00          NOP     
0263: 00          NOP     
0264: 00          NOP     
0265: 00          NOP     
0266: 00          NOP     
0267: 00          NOP     
0268: 00          NOP     
0269: 00          NOP     
026a: 00          NOP     
026b: 00          NOP     
026c: 00          NOP     
026d: 00          NOP     
026e: 00          NOP     
026f: 00          NOP     
0270: 00          NOP     
0271: 00          NOP     
0272: 00          NOP     
0273: 00          NOP     
0274: 00          NOP     
0275: 00          NOP     
0276: 00          NOP     
0277: 00          NOP     
0278: 00          NOP     
0279: 00          NOP     
027a: 00          NOP     
027b: 00          NOP     
027c: 00          NOP     
027d: 00          NOP     
027e: 00          NOP     
027f: 00          NOP     

cold start
0280: 69 0e       MVI     A,$0E
0282: 4d d0       MOV     MM,A
0284: 68 ff       MVI     V,$FF
0286: 10          EXA     
0287: 68 ff       MVI     V,$FF
0289: 04 ff ff    LXI     SP,$FFFF
028c: 69 00       MVI     A,$00
028e: 4d d2       MOV     MA,A
0290: 4d d3       MOV     MB,A
0292: 4d d4       MOV     MC,A
0294: 4d d7       MOV     MF,A
0296: 4d c8       MOV     ANM,A
0298: 34 00 ff    LXI     HL,$FF00       // bottom of RAM
029b: 3d          STAX    (HL+)
029c: 74 7f 00    EQI     L,$00
029f: fb          JR      $029B          // loop around until RAM is cleared
02a0: 69 02       MVI     A,$02
02a2: 4d d1       MOV     MCC,A
02a4: 69 4e       MVI     A,$4E
02a6: 4d ca       MOV     SML,A
02a8: 69 0a       MVI     A,$0A
02aa: 4d c9       MOV     SMH,A
02ac: 34 09 ff    LXI     HL,$FF09_voice1Note
02af: 6b 05       MVI     C,$05          // six voices
02b1: 69 3c       MVI     A,$3C          // Middle C
02b3: 3d          STAX    (HL+)          // initialise Note setting
02b4: 53          DCR     C
02b5: fd          JR      $02B3          // loop around
02b6: 64 05 40    MVI     PF,$40         // 8' divider
02b9: 71 1e 40    MVIW    $FF1E_flags1,$40 // sqr off/on
02bc: 71 46 15    MVIW    $FF46_ic40Latch,$15
02bf: 71 37 06    MVIW    $FF37_switch2,$06
02c2: 14 80 3f    LXI     BC,$3F80       // env, cutoff to maximum
02c5: 70 1e 23 ff SBCD    $FF23_envSustainLev
02c9: 70 1e 3d ff SBCD    $FF3D_vcfCutoff
02cd: 14 00 20    LXI     BC,$2000       // VCA about half way up
02d0: 70 1e 43 ff SBCD    $FF43_vcaMaster
02d4: 14 00 02    LXI     BC,$0200       // LFO rate slow
02d7: 70 1e 4b ff SBCD    $FF4B_lfoRateCoeff
02db: 34 00 23    LXI     HL,$2300       // program dividers
02de: 78 25       CALF    $0825_setModeWord
02e0: 34 00 13    LXI     HL,$1300
02e3: 78 25       CALF    $0825_setModeWord
02e5: 48 48       SKIT    FAD
02e7: 00          NOP     
02e8: 64 06 04    MVI     MKH,$04
02eb: aa          EI

here's the main loop
02ec: 01 46       LDAW    $FF46_ic40Latch
02ee: 70 79 00 30 MOV     ($3000),A          // set chorus and HPF outputs
02f2: 58 1e       BIT     0,$FF1E_flags1     // sustained?
02f4: c8          JR      $02FD              // no
02f5: 01 11       LDAW    $FF11_voiceRun     // get which notes are running
02f7: 74 98 10    ORAW    $FF10_noteGate     // OR in the current gate
02fa: 63 11       STAW    $FF11_voiceRun     // save it back
02fc: c4          JR      $0301              // and continue
02fd: 01 10       LDAW    $FF10_noteGate     // sustain is off, just load the gated voice
02ff: 63 11       STAW    $FF11_voiceRun     // into the running voice value

0301: 34 3f ff    LXI     HL,$FF3F_vcfReso
0304: 48 83       LDEAX   (HL)
0306: 69 06       MVI     A,$06              // VCF Resonance CV output
0308: 78 2f       CALF    $082F_loadDac
030a: 64 08 bf    ANI     PA,$BF             // IC26 VCA CV mux
030d: 45 11 3f    ONIW    $FF11_voiceRun,$3F          // are any voices started
0310: 4e 59       JRE     $036B              // no
0312: 5b 1e       BIT     3,$FF1E_flags1     // yes but we've already started the LFO delay
0314: ce          JR      $0323
0315: 14 00 00    LXI     BC,$0000           // no
0318: 70 1e 56 ff SBCD    $FF56_lfoHoldOff   // zero out the holdoff time and envelope
031c: 70 1e 5a ff SBCD    $FF5A_lfoDelayEnv
0320: 05 1e f1    ANIW    $FF1E_flags1,$F1           // clear LFO delay bits        
0323: 59 1e       BIT     1,$FF1E_flags1             // LFO holdoff time
0325: 4e 49       JRE     $0370
0327: 5a 1e       BIT     2,$FF1E_flags1             // LFO attack time
0329: 4e 5d       JRE     $0388
032b: 6a ff       MVI     B,$FF                      // maximum LFO amount
032d: b1          PUSH    BC
032e: 01 49       LDAW    $FF49_lfoToPitchScaler     // 
0330: 48 2e       MUL     B
0332: 08          MOV     A,EAH
0333: 74 a0 64    ADDNCW  $FF64_lfoModToPitch
0336: 69 ff       MVI     A,$FF
0338: 34 51 ff    LXI     HL,$FF51_lfoToPitch
033b: 70 1f 4d ff LBCD    $FF4D_lfoVal
033f: 48 2f       MUL     C
0341: 1c          MOV     D,A
0342: 08          MOV     A,EAH
0343: 1b          MOV     C,A
0344: 0c          MOV     A,D
0345: 48 2e       MUL     B
0347: 70 43       EADD    EA,C
0349: 48 a0       DSLR    EA
034b: 48 a0       DSLR    EA
034d: 48 a0       DSLR    EA
034f: 48 95       STEAX   (HL++)
0351: a1          POP     BC
0352: 01 48       LDAW    $FF48_lfoToVcf
0354: 48 2e       MUL     B
0356: 08          MOV     A,EAH
0357: 70 1f 4d ff LBCD    $FF4D_lfoVal
035b: 48 2f       MUL     C
035d: 1c          MOV     D,A
035e: 08          MOV     A,EAH
035f: 1b          MOV     C,A
0360: 0c          MOV     A,D
0361: 48 2e       MUL     B
0363: 70 43       EADD    EA,C
0365: 48 a0       DSLR    EA
0367: 48 93       STEAX   (HL)
0369: 4e 36       JRE     $03A1
036b: 15 1e 08    ORIW    $FF1E_flags1,$08
036e: 4f b3       JRE     $0323
0370: 24 56 ff    LXI     DE,$FF56_lfoHoldOff
0373: 48 82       LDEAX   (DE)
0375: 70 1f 58 ff LBCD    $FF58_lfoDelayCoeff
0379: 74 c5       DADD    EA,BC
037b: 48 92       STEAX   (DE)
037d: 08          MOV     A,EAH
037e: 57 c0       OFFI    A,$C0
0380: c4          JR      $0385
0381: 6a 00       MVI     B,$00
0383: 4f a8       JRE     $032D
0385: 15 1e 02    ORIW    $FF1E_flags1,$02
0388: 34 5a ff    LXI     HL,$FF5A_lfoDelayEnv
038b: 48 83       LDEAX   (HL)
038d: 70 1f 6c ff LBCD    $FF6C_lfoHoldoffCoeff
0391: 74 a5       DADDNC  EA,BC
0393: c6          JR      $039A
0394: 48 93       STEAX   (HL)
0396: 08          MOV     A,EAH
0397: 1a          MOV     B,A
0398: 4f 93       JRE     $032D
039a: 15 1e 04    ORIW    $FF1E_flags1,$04
039d: 6a ff       MVI     B,$FF
039f: 4f 8c       JRE     $032D
03a1: 69 06       MVI     A,$06                      // VCA Master gain
03a3: 34 43 ff    LXI     HL,$FF43_vcaMaster
03a6: 48 83       LDEAX   (HL)
03a8: 78 2f       CALF    $082F_loadDac
03aa: 64 08 df    ANI     PA,$DF                     // IC23 VCF CV mux
03ad: 44 18 18    LXI     EA,$1818                   // basic pitch value
03b0: 01 61       LDAW    $FF61_tuningAmt            // synth tune pot
03b2: 5f 1e       BIT     7,$FF1E_flags1             // positive/negative value
03b4: c3          JR      $03B8                      // add or subtract tuning amount
03b5: 70 61       ESUB    EA,A
03b7: c2          JR      $03BA
03b8: 70 41       EADD    EA,A
03ba: 70 1f 51 ff LBCD    $FF51_lfoToPitch           // computed LFO amount
03be: 59 4a       BIT     1,$FF4A_lfoFlag             // LFO polarity
03c0: c3          JR      $03C4                      // add or subtract
03c1: 74 e5       DSUB    EA,BC
03c3: c2          JR      $03C6
03c4: 74 c5       DADD    EA,BC
03c6: 70 1f 68 ff LBCD    $FF68                      // bend amount?
03ca: 5d 1e       BIT     5,$FF1E_flags1             // bend polarity
03cc: c3          JR      $03D0                      // add or subtract
03cd: 74 e5       DSUB    EA,BC
03cf: c2          JR      $03D2
03d0: 74 c5       DADD    EA,BC
03d2: 24 6f ff    LXI     DE,$FF6F_tuneLfoBend       // basic pitch offset
03d5: 48 92       STEAX   (DE)
03d7: 71 0f 00    MVIW    $FF0F_voicePtr,$00         // first voice
03da: 24 71 ff    LXI     DE,$FF71_notePitchFrac     // now compute the portamento
03dd: 34 09 ff    LXI     HL,$FF09_voice1Note
03e0: 48 82       LDEAX   (DE)                       // fetch the current voice tuning
03e2: 2d          LDAX    (HL+)                      // and the desired note
03e3: 1a          MOV     B,A                        
03e4: 6b 00       MVI     C,$00                      // BC now holds the desired note pitch, no fraction
03e6: 01 7d       LDAW    $FF7D_portaCoeff           // and the portament amount
03e8: 77 00       EQI     A,$00                      // if it's zero skip
03ea: ca          JR      $03F5                      // actually do the portamento
03eb: a5          DMOV    EA,BC                      // just put the pitch straight in
03ec: 48 94       STEAX   (DE++)                     // and save it
03ee: 20 0f       INRW    $FF0F_voicePtr             // next voice
03f0: 75 0f 06    EQIW    $FF0F_voicePtr,$06         // done all six?
03f3: ec          JR      $03E0                      // nope, back around to porta the next one
03f4: d2          JR      $0407                      // jump on forwards, nothing more to do
calculate portamento
03f5: 74 ed       DNE     EA,BC                      // if the porta pitch is equal to the desired pitch
03f7: f4          JR      $03EC                      // jump back and save it
03f8: 74 ad       DGT     EA,BC                      // greater?
03fa: c6          JR      $0401                      // no
03fb: 70 61       ESUB    EA,A                       // yes, subtract 
03fd: 74 ad       DGT     EA,BC                      // if it's still greater...
03ff: a5          DMOV    EA,BC                      // it isn't, we went too far, set to the target pitch
0400: eb          JR      $03EC                      // ... jump back and save
0401: 70 41       EADD    EA,A                       // add porta amount
0403: 74 bd       DLT     EA,BC                      // if it's still less than target...
0405: a5          DMOV    EA,BC                      // it isn't, we went too far, set to the target pitch
0406: e5          JR      $03EC                      // ... jump back and save

set sub osc level
0407: 69 06       MVI     A,$06
0409: 34 3b ff    LXI     HL,$FF3B_subOscLev
040c: 48 83       LDEAX   (HL)
040e: 78 2f       CALF    $082F_loadDac
0410: 64 08 ef    ANI     PA,$EF                     // IC24 VCO Ramp CV mux

now set the oscillator pitches
0413: 71 0f 00    MVIW    $FF0F_voicePtr,$00     // first voice
0416: 71 34 01    MVIW    $FF34_activeVoiceBit,$01              // voice selector bit
0419: 24 71 ff    LXI     DE,$FF71_notePitchFrac // first notepitch amount
041c: 48 84       LDEAX   (DE++)                 // EA contains pitch, bump DE
041e: 70 1f 6f ff LBCD    $FF6F_tuneLfoBend      // precalculated pitch offest
0422: 74 c5       DADD    EA,BC                  // add it
0424: 09          MOV     A,EAL                  
0425: 63 6e       STAW    $FF63_midiModDepth     // save the fractional amount for interpolation, this variable is not needed now
0427: 08          MOV     A,EAH                  // get the note number
0428: 27 2f       GTI     A,$2F                  // greater than 47
042a: 4e 79       JRE     $04A5                  // no
042c: 37 97       LTI     A,$97                  // less than 151
042e: 4e 7c       JRE     $04AC                  // no
0430: 66 30       SUI     A,$30                  // subtract 48 
0432: 48 25       SLL     A                      // double it
0434: 44 60 0e    LXI     EA,$0E60_noteCvDacTbl  // point to the table of CV DAC values
0437: 70 41       EADD    EA,A                   // add the note...
0439: b4          PUSH    EA                     // .. and save it on the stack for now
043a: 44 30 0f    LXI     EA,$0F30_noteClkDivTbl // point to the table of 8253 divider values
043d: 70 41       EADD    EA,A                   // add the note...
043f: b7          DMOV    HL,EA
0440: 48 8f 02    LDEAX   (HL+$02)               // get the value of the note above
0443: b5          DMOV    BC,EA                  // into BC
0444: 48 83       LDEAX   (HL)                   // get this note
0446: b4          PUSH    EA                     // save it on the stack for later
0447: 74 e5       DSUB    EA,BC
0449: b5          DMOV    BC,EA                  // difference between the two in BC
044a: 01 6e       LDAW    $FF63_midiModDepth     // multiply by the fractional amount to interpolate
044c: 48 2f       MUL     C
044e: 08          MOV     A,EAH
044f: 1b          MOV     C,A
0450: 01 6e       LDAW    $FF63_midiModDepth
0452: 48 2e       MUL     B
0454: 70 43       EADD    EA,C
0456: b5          DMOV    BC,EA
0457: a4          POP     EA
0458: 74 e5       DSUB    EA,BC                  // I think the interpolate value is negative, subtract
045a: 01 0f       LDAW    $FF0F_voicePtr         // which voice?
045c: 1a          MOV     B,A
045d: 01 34       LDAW    $FF34_activeVoiceBit                  // voice bit
045f: 74 d8 00    OFFAW   $FF00_resetVoiceBits   // does it need reset
0462: 4e 4f       JRE     $04B3                  // yes, reset it
0464: ba          DI                             // this could go squint if an interrupt happens right now
0465: 34 50 0b    LXI     HL,$0B50               // get address for clock chip
0468: ad          LDAX    (HL+B)
0469: 1e          MOV     H,A                    // form clock chip address
046a: 6f 00       MVI     L,$00
046c: 09          MOV     A,EAL                  // low byte of divider first
046d: 3b          STAX    (HL)                   // send it to the chip
046e: 08          MOV     A,EAH                  // high byte of divider
046f: 3b          STAX    (HL)                   // send it to the chip
0470: aa          EI                             // phew
0471: a3          POP     HL                     // the DAC table address that was in EA, get it in HL
0472: 48 85       LDEAX   (HL++)                 // load into EA, and increment HL to next entry
0474: b5          DMOV    BC,EA                  // copy to BC
0475: b4          PUSH    EA                     // save on stack too
0476: 48 83       LDEAX   (HL)                   // get the value after it
0478: 74 e5       DSUB    EA,BC
047a: b5          DMOV    BC,EA                  // calculate the interpolated amount
047b: 01 6e       LDAW    $FF63_midiModDepth
047d: 48 2f       MUL     C
047f: 08          MOV     A,EAH
0480: 1b          MOV     C,A
0481: 01 6e       LDAW    $FF63_midiModDepth
0483: 48 2e       MUL     B
0485: 70 43       EADD    EA,C
0487: b5          DMOV    BC,EA
0488: a4          POP     EA
0489: 74 c5       DADD    EA,BC                  // add in the original amount
048b: 48 a4       DSLL    EA
048d: 48 a4       DSLL    EA                     // left align it for the DAC
048f: 01 0f       LDAW    $FF0F_voicePtr         // pick up the voice number
0491: 78 2f       CALF    $082F_loadDac        // send it to the DAC
0493: 64 08 ef    ANI     PA,$EF                 // IC24 VCO Ramp CV mux
0496: 01 34       LDAW    $FF34_activeVoiceBit                  // voice bit
0498: 48 25       SLL     A                      // move to next voice
049a: 63 34       STAW    $FF34_activeVoiceBit                  // save it back
049c: 20 0f       INRW    $FF0F_voicePtr         // next voice
049e: 75 0f 06    EQIW    $FF0F_voicePtr,$06     // done all six?
04a1: 4f 79       JRE     $041C                  // no, go back again
04a3: 4e 30       JRE     $04D5                  // yes, go on and compute VCF values
upper and lower limits for note number
too low, zero out fractional part and note number
04a5: 71 6e 00    MVIW    $FF63_midiModDepth,$00
04a8: 69 00       MVI     A,$00
04aa: 4f 86       JRE     $0432                      // go back
too high, zero out fractional part and set maximum note number
04ac: 71 6e 00    MVIW    $FF63_midiModDepth,$00
04af: 69 66       MVI     A,$66
04b1: 4f 7f       JRE     $0432                      // go back
reset voice oscillator
04b3: 16 ff       XRI     A,$FF                      // mask off reset bit
04b5: 74 88 00    ANAW    $FF00_resetVoiceBits
04b8: 63 00       STAW    $FF00_resetVoiceBits
04ba: 34 56 0b    LXI     HL,$0B56                   // lookup table for reset command
04bd: ad          LDAX    (HL+B)
04be: 6e 23       MVI     H,$23                      // either first chip
04c0: 74 3a 03    LTI     B,$03
04c3: 6e 13       MVI     H,$13                      // or second
04c5: 6f 00       MVI     L,$00
04c7: ba          DI                                 // don't let anything interrupt now
04c8: 3b          STAX    (HL)                       // send the reset command
04c9: 4f 9a       JRE     $0465                      // jump back and program the clock chip

I guess this is to fit an extra instruction in
seems odd, only ever jumped to from the VCF calculations
04cb: 74 c5       DADD    EA,BC
04cd: db          JR      $04E9

to do with calculating VCF, does it overflow?
04ce: 74 a5       DADDNC  EA,BC
04d0: 71 6a 00    MVIW    $FF6A_vcfOverflow1,$00
04d3: 4e 21       JRE     $04F6

calculate VCF amounts
04d5: 71 6a 00    MVIW    $FF6A_vcfOverflow1,$00      // zero out overflow flag
04d8: 70 1f 3d ff LBCD    $FF3D_vcfCutoff            // cutoff knob scaled to $0000-$3f80
04dc: a5          DMOV    EA,BC
04dd: 70 1f 53 ff LBCD    $FF53_vcfLfoSignal
04e1: 59 4a       BIT     1,$FF4A_lfoFlag            // polarity
04e3: e7          JR      $04CB                      // just adds EA to BC
04e4: 74 b5       DSUBNB  EA,BC                      // or subtracts, depending on polarity
04e6: 71 6a 01    MVIW    $FF6A_vcfOverflow1,$01     // if it underflowed set flag
04e9: 70 1f 65 ff LBCD    $FF65_vcfBendAmt
04ed: 5d 1e       BIT     5,$FF1E_flags1             // bend polarity
04ef: 4f dd       JRE     $04CE                      // add, checking for overflow
04f1: 74 b5       DSUBNB  EA,BC                      // subtract...
04f3: 71 6a 01    MVIW    $FF6A_vcfOverflow1,$01     // ... and flagging underflow
04f6: b4          PUSH    EA                         // EA = Cutoff knob + LFO + bend
04f7: 71 0f 00    MVIW    $FF0F_voicePtr,$00         // first voice
04fa: 71 34 01    MVIW    $FF34_activeVoiceBit,$01
04fd: 34 71 ff    LXI     HL,$FF71_notePitchFrac     // first note and fractional pitch
0500: 24 25 ff    LXI     DE,$FF25_envRlsCoeff       // wonder why they did it this way
0503: b3          PUSH    HL                         // save address on stack
0504: 48 8b 02    LDEAX   (DE+$02)                   // fetch envelope for voice
0507: 01 34       LDAW    $FF34_activeVoiceBit       // voice bit selector
0509: 74 c8 11    ONAW    $FF11_voiceRun             // is it running?
050c: 4e 2a       JRE     $0538
050e: 74 c8 33    ONAW    $FF33_rlsPhase             // is it in release phase?
0511: 4e 50       JRE     $0563
0513: 74 c8 07    ONAW    $FF07_atkPhase             // is it in attack phase
0516: c7          JR      $051E
//
calculate decay phase for envelope
0517: 16 ff       XRI     A,$FF                      // invert mask
0519: 74 88 07    ANAW    $FF07_atkPhase             // clear attack phase bit
051c: 63 07       STAW    $FF07_atkPhase
051e: 70 1f 23 ff LBCD    $FF23_envSustainLev        // fetch sustain level
0522: 74 ad       DGT     EA,BC                      // if envelope level > sustain
0524: a5          DMOV    EA,BC                      // it's not, set them the same
0525: 74 e5       DSUB    EA,BC                      // subtract sustain level
0527: b5          DMOV    BC,EA                      // move to BC
0528: 01 22       LDAW    $FF22_envDcyHighByte       // Decay coefficient from table, MSB
052a: 63 35       STAW    $FF35_dcyTemp              // save it in variable for env calculator
052c: 01 21       LDAW    $FF21_envDcyCoeff          // LSB
052e: 78 3d       CALF    $083D_calcDecay            // do the multiply
0530: 70 1f 23 ff LBCD    $FF23_envSustainLev        // fetch the sustain level
0534: 74 c5       DADD    EA,BC                      // add it back in
0536: 4e 58       JRE     $0590                      // go round and do the next voice
0538: 74 c8 07    ONAW    $FF07_atkPhase
053b: ce          JR      $054A
053c: 74 d8 08    OFFAW   $FF08_dcyPhase
053f: 4f d6       JRE     $0517
0541: 1a          MOV     B,A
0542: 16 ff       XRI     A,$FF
0544: 74 88 07    ANAW    $FF07_atkPhase
0547: 63 07       STAW    $FF07_atkPhase
0549: 0a          MOV     A,B
054a: 16 ff       XRI     A,$FF
054c: 1a          MOV     B,A
054d: 74 88 33    ANAW    $FF33_rlsPhase
0550: 63 33       STAW    $FF33_rlsPhase
0552: 0a          MOV     A,B
0553: 74 88 08    ANAW    $FF08_dcyPhase
0556: 63 08       STAW    $FF08_dcyPhase
0558: b5          DMOV    BC,EA
0559: 01 26       LDAW    $FF26_envRlsLowByte
055b: 63 35       STAW    $FF35_dcyTemp
055d: 01 25       LDAW    $FF25_envRlsCoeff
055f: 78 3d       CALF    $083D_calcDecay
0561: 4e 2d       JRE     $0590
//
attack phase
0563: 01 34       LDAW    $FF34_activeVoiceBit
0565: 16 ff       XRI     A,$FF
0567: 74 88 08    ANAW    $FF08_dcyPhase
056a: 63 08       STAW    $FF08_dcyPhase             // mask off decay bit
056c: 01 45       LDAW    $FF45_envAtkVal            // attack knob $00-$7f
056e: 48 25       SLL     A                          // form a pointer to a two-byte table
0570: 34 60 0b    LXI     HL,$0B60_envAtkTbl
0573: 48 8c       LDEAX   (HL+A)                     // fetch attack time coeff from table
0575: b5          DMOV    BC,EA                      // save in BC
0576: 48 8b 02    LDEAX   (DE+$02)                   // fetch current envelope value
0579: 74 c5       DADD    EA,BC                      // add it on
057b: 08          MOV     A,EAH
057c: 47 c0       ONI     A,$C0                      // EA > 0x3fff?
057e: d1          JR      $0590                      // no, continue
057f: 44 ff 3f    LXI     EA,$3FFF                   // clamp to maximum
0582: 01 34       LDAW    $FF34_activeVoiceBit       // this voice...
0584: 74 98 33    ORAW    $FF33_rlsPhase             // now has the release phase
0587: 63 33       STAW    $FF33_rlsPhase
0589: 01 34       LDAW    $FF34_activeVoiceBit
058b: 74 98 08    ORAW    $FF08_dcyPhase             // and decay phase bits set
058e: 63 08       STAW    $FF08_dcyPhase
go on to do next envelope
0590: 48 9b 02    STEAX   (DE+$02)                   // save
0593: 48 84       LDEAX   (DE++)                     // get value back, increment DE
0595: 75 0f 00    EQIW    $FF0F_voicePtr,$00         // if this is our first time round the loop...
0598: cd          JR      $05A6                      // it isn't
0599: 70 1f 4f ff LBCD    $FF4F_pwmSignal            // send either PWM or PWMLFO to the DAC
059d: a5          DMOV    EA,BC
059e: 69 07       MVI     A,$07                      // PWM channel
05a0: 78 2f       CALF    $082F_loadDac
05a2: 64 08 df    ANI     PA,$DF                     // IC23 VCF CV mux
05a5: da          JR      $05C0
//
envelope/gate settings
05a6: 5a 37       BIT     2,$FF37_switch2
05a8: c9          JR      $05B2
05a9: 01 0f       LDAW    $FF0F_voicePtr
05ab: 51          DCR     A
05ac: 78 2f       CALF    $082F_loadDac
05ae: 64 08 bf    ANI     PA,$BF                     // IC26 VCA CV mux
05b1: ce          JR      $05C0
05b2: 01 34       LDAW    $FF34_activeVoiceBit
05b4: 48 21       SLR     A
05b6: 44 ff 3f    LXI     EA,$3FFF
05b9: 74 c8 11    ONAW    $FF11_voiceRun
05bc: 44 00 00    LXI     EA,$0000
05bf: e9          JR      $05A9

05c0: a3          POP     HL                         // get pitch address back
05c1: 01 6a       LDAW    $FF6A_vcfOverflow1
05c3: 63 6b       STAW    $FF6B_vcfOverflow2
05c5: 48 82       LDEAX   (DE)
05c7: b5          DMOV    BC,EA
05c8: 01 41       LDAW    $FF41_vcfEnvMod            // env mod knob from 0-254
05ca: 48 2f       MUL     C
05cc: 08          MOV     A,EAH
05cd: 1b          MOV     C,A
05ce: 01 41       LDAW    $FF41_vcfEnvMod
05d0: 48 2e       MUL     B
05d2: 70 43       EADD    EA,C
05d4: b5          DMOV    BC,EA                      // BC now contains scaled envelope for VCF
05d5: a4          POP     EA                         // get base VCF setting back
05d6: b4          PUSH    EA
05d7: 59 37       BIT     1,$FF37_switch2            // positive or negative envelope
05d9: 4e 3e       JRE     $0619
05db: 74 a5       DADDNC  EA,BC
05dd: 71 6b 00    MVIW    $FF6B_vcfOverflow2,$00
                                                     // now the key follow
05e0: b4          PUSH    EA                         // save base VCF +/- envelope
05e1: 48 85       LDEAX   (HL++)                     // fetch pitch, bump HL
05e3: 48 a0       DSLR    EA                         // divide by 2
05e5: 48 a0       DSLR    EA                         // divide by 4
05e7: b5          DMOV    BC,EA                      // save
05e8: 48 a0       DSLR    EA                         // divide by 8
05ea: 74 c5       DADD    EA,BC                      // add, npw is EA*0.375
05ec: 14 80 16    LXI     BC,$1680                   // middle C * 0.375           
05ef: 74 ad       DGT     EA,BC                      // greater?
05f1: 4e 2d       JRE     $0620                      // no
05f3: 74 e5       DSUB    EA,BC                      // difference
05f5: b5          DMOV    BC,EA
05f6: 01 42       LDAW    $FF42_vcfKeyTrack          // key follow knob from $00-$fe
05f8: 48 2f       MUL     C
05fa: 08          MOV     A,EAH
05fb: 1b          MOV     C,A
05fc: 01 42       LDAW    $FF42_vcfKeyTrack
05fe: 48 2e       MUL     B
0600: 70 43       EADD    EA,C
0602: b5          DMOV    BC,EA                      // multiply
0603: a4          POP     EA                         // fetch VCF for this channel
0604: 74 a5       DADDNC  EA,BC                      // add key follow
0606: 71 6b 00    MVIW    $FF6B_vcfOverflow2,$00
0609: 08          MOV     A,EAH
060a: 47 c0       ONI     A,$C0                      // if it overflowed
060c: 4e 2e       JRE     $063C                      // (it didn't, jump)
060e: 44 00 00    LXI     EA,$0000                   // clamp to zero
0611: 45 6b 01    ONIW    $FF6B_vcfOverflow2,$01     // Was the overflow flag set?
0614: 44 ff 3f    LXI     EA,$3FFF                   // yes, clamp VCF to upper bound
0617: 4e 23       JRE     $063C                      // jump ahead
subtract envelope from base VCF
0619: 74 b5       DSUBNB  EA,BC
061b: 71 6b 01    MVIW    $FF6B_vcfOverflow2,$01
061e: 4f c0       JRE     $05E0                      // jump back and do key follow
//
0620: b5          DMOV    BC,EA                      // subtract key track amount
0621: 44 80 16    LXI     EA,$1680
0624: 74 e5       DSUB    EA,BC
0626: b5          DMOV    BC,EA
0627: 01 42       LDAW    $FF42_vcfKeyTrack
0629: 48 2f       MUL     C
062b: 08          MOV     A,EAH
062c: 1b          MOV     C,A
062d: 01 42       LDAW    $FF42_vcfKeyTrack
062f: 48 2e       MUL     B
0631: 70 43       EADD    EA,C
0633: b5          DMOV    BC,EA
0634: a4          POP     EA
0635: 74 b5       DSUBNB  EA,BC
0637: 71 6b 01    MVIW    $FF6B_vcfOverflow2,$01
063a: 4f cd       JRE     $0609                      // jump back, check for overflow
063c: 01 0f       LDAW    $FF0F_voicePtr             // DAC channel
063e: 78 2f       CALF    $082F_loadDac
0640: 64 08 df    ANI     PA,$DF                     // IC23 VCF CV mux
0643: 01 34       LDAW    $FF34_activeVoiceBit       // next voice bit
0645: 48 25       SLL     A
0647: 63 34       STAW    $FF34_activeVoiceBit
0649: 20 0f       INRW    $FF0F_voicePtr             // next voice
064b: 75 0f 06    EQIW    $FF0F_voicePtr,$06         // have we done six?
064e: 54 03 05    JMP     $0503                      // no, loop
0651: a4          POP     EA                         // discard base envelope setting
0652: b2          PUSH    DE                         // save envelope pointer?

calculate pitch bend
0653: 34 5d ff    LXI     HL,$FF5D_adcChan0
0656: 5b 5c       BIT     3,$FF5C_adcMode            // are we doing the first or second set
0658: 4e 88       JRE     $06E2                      // do first set
065a: 24 84 ff    LXI     DE,$FF84_dacOut4           // VCF Bend Sense onwards
065d: 78 00       CALF    $0800_handleAdcVal         // work out hysteresis
065f: 01 8e       LDAW    $FF8E_dacBendPol           // has the bender polarity changed?
0661: 74 90 02    XRAW    $FF02_bendPol
0664: 57 80       OFFI    A,$80
0666: 4e 55       JRE     $06BD                      // has changed
0668: 01 8f       LDAW    $FF8F_dacBendCv             
066a: 74 f8 03    EQAW    $FF03_bendVal1             // has the bender value changed
066d: 4e 4e       JRE     $06BD                      // yes
066f: 55 05 01    OFFIW   $FF05_bendPolarity,$01
0672: 4e 5c       JRE     $06D0
0674: 01 06       LDAW    $FF06_bendVal2
0676: 1a          MOV     B,A                    // into B to multiply it
0677: 1c          MOV     D,A                    // save a copy in D
0678: 01 8c       LDAW    $FF8C_dacVcfBendSens
067a: 48 2e       MUL     B                      // multiply by VCF bend amount
067c: 48 a0       DSLR    EA                     // divide by 16
067e: 48 a0       DSLR    EA
0680: 48 a0       DSLR    EA
0682: 48 a0       DSLR    EA
0684: 34 65 ff    LXI     HL,$FF65_vcfBendAmt    // stored as a 12-bit value now
0687: 48 95       STEAX   (HL++)                 // store, increment HL to point at $FF67
0689: 0c          MOV     A,D                    // get back the bend value
068a: 1a          MOV     B,A                    // into B to multiply it
068b: 01 8d       LDAW    $FF8D_dacDcoBendSens
068d: 48 2e       MUL     B                      // multiply by DCO bend amount
068f: 08          MOV     A,EAH                  
0690: 1c          MOV     D,A                    // keep upper byte in D
0691: 48 a0       DSLR    EA                     // divide by 2
0693: b5          DMOV    BC,EA                  // save
0694: 48 a0       DSLR    EA                     // divide by 2 again
0696: 74 c5       DADD    EA,BC                  // add, now it's multiplied by 0.75
0698: 0c          MOV     A,D                    // get original back
0699: 70 41       EADD    EA,A                   // multipled by 1.75
okay this is one of those things in disassemblies where the "what" is obvious
but the "why" remains elusive
069b: 48 2a       CLC                            // clear carry                    
069d: 69 00       MVI     A,$00                  // zero out accumulator
069f: 48 a4       DSLL    EA                     // shift top bit into carry
06a1: 48 35       RLL     A                      // shift carry into A
06a3: 48 a4       DSLL    EA
06a5: 48 35       RLL     A
06a7: 48 a4       DSLL    EA
06a9: 48 35       RLL     A
06ab: 48 a4       DSLL    EA                     // do this four times, now EA *= 16
06ad: 48 35       RLL     A                      // and A contains the top four bits
06af: 63 69       STAW    $FF69                  // save A
06b1: 48 93       STEAX   (HL)                   // save EA at FF67/FF68
wait for DAC conversion to be complete
06b3: ba          DI      
06b4: 48 48       SKIT    FAD                    // 
06b6: 00          NOP     
06b7: 64 06 04    MVI     MKH,$04                // unmask serial and AD ints
06ba: aa          EI      
06bb: 4e 7a       JRE     $0737

06bd: 01 8e       LDAW    $FF8E_dacBendPol
06bf: 63 02       STAW    $FF02_bendPol
06c1: 05 1e df    ANIW    $FF1E_flags1,$DF
06c4: 57 80       OFFI    A,$80
06c6: 15 1e 20    ORIW    $FF1E_flags1,$20
06c9: 01 8f       LDAW    $FF8F_dacBendCv
06cb: 63 03       STAW    $FF03_bendVal1
06cd: 63 06       STAW    $FF06_bendVal2
06cf: cd          JR      $06DD

06d0: 05 1e df    ANIW    $FF1E_flags1,$DF
06d3: 55 1e 10    OFFIW   $FF1E_flags1,$10 // bend positive
06d6: 15 1e 20    ORIW    $FF1E_flags1,$20
06d9: 01 62       LDAW    $FF62_midiBendAmt
06db: 63 06       STAW    $FF06_bendVal2
06dd: 71 05 00    MVIW    $FF05_bendPolarity,$00
06e0: 4f 94       JRE     $0676
06e2: 24 80 ff    LXI     DE,$FF80_dacOut0
06e5: 78 00       CALF    $0800_handleAdcVal
06e7: 01 89       LDAW    $FF89_dacPorta
06e9: 67 00       NEI     A,$00
06eb: c6          JR      $06F2
06ec: 48 21       SLR     A
06ee: 34 00 0a    LXI     HL,$0A00_portCoeffTbl
06f1: ac          LDAX    (HL+A)
06f2: 63 7d       STAW    $FF7D_portaCoeff
06f4: 45 8a 80    ONIW    $FF8A_dacLfoTrig,$80
06f7: 4e 2d       JRE     $0726
06f9: 71 7e 00    MVIW    $FF7E_modSwDeclick,$00
06fc: 69 00       MVI     A,$00
06fe: 1b          MOV     C,A
06ff: 01 8b       LDAW    $FF8B_dacLfoSens
0701: 1a          MOV     B,A
0702: 01 63       LDAW    $FF63_midiModDepth
0704: 48 2e       MUL     B
0706: 08          MOV     A,EAH
0707: 60 ab       GTA     A,C
0709: 0b          MOV     A,C
070a: 63 64       STAW    $FF64_lfoModToPitch
070c: 01 88       LDAW    $FF88_dacTune
070e: 47 80       ONI     A,$80
0710: c9          JR      $071A
0711: 07 7f       ANI     A,$7F
0713: 63 61       STAW    $FF61_tuningAmt
0715: 05 1e 7f    ANIW    $FF1E_flags1,$7F
0718: 4f 99       JRE     $06B3
071a: 1a          MOV     B,A
071b: 69 80       MVI     A,$80
071d: 60 e2       SUB     A,B
071f: 63 61       STAW    $FF61_tuningAmt
0721: 15 1e 80    ORIW    $FF1E_flags1,$80
0724: 4f 8d       JRE     $06B3

this seems to "de-click" turning on the modwheel
0726: 01 8b       LDAW    $FF8B_dacLfoSens
0728: 74 a8 7e    GTAW    $FF7E_modSwDeclick
072b: 4f d1       JRE     $06FE
072d: 01 7e       LDAW    $FF7E_modSwDeclick
072f: 26 0a       ADINC   A,$0A
0731: 69 ff       MVI     A,$FF
0733: 63 7e       STAW    $FF7E_modSwDeclick
0735: 4f c7       JRE     $06FE

0737: a2          POP     DE                     // get back the envelope address
0738: 48 82       LDEAX   (DE)                   // get the envelope value
073a: 5a 37       BIT     2,$FF37_switch2        // env/gate switch
073c: c1          JR      $073E                  // gate mode
073d: c9          JR      $0747                  // env mode
073e: 44 ff 3f    LXI     EA,$3FFF               // max level
0741: 45 11 20    ONIW    $FF11_voiceRun,$20     // is voice 6 running? Why specifically voice 6?
0744: 44 00 00    LXI     EA,$0000               // no, min level
0747: 69 05       MVI     A,$05                  // voice 6 VCA channel
0749: 78 2f       CALF    $082F_loadDac          
074b: 64 08 bf    ANI     PA,$BF                 // IC26 VCA CV mux
074e: 34 4d ff    LXI     HL,$FF4D_lfoVal
0751: 48 83       LDEAX   (HL)
0753: 70 1f 4b ff LBCD    $FF4B_lfoRateCoeff
0757: 58 4a       BIT     0,$FF4A_lfoFlag // rise/fall
0759: 4e 30       JRE     $078B
075b: 74 b5       DSUBNB  EA,BC
075d: 4e 3b       JRE     $079A
075f: 48 95       STEAX   (HL++)
0761: 59 4a       BIT     1,$FF4A_lfoFlag // polarity
0763: 4e 3d       JRE     $07A2
0765: b5          DMOV    BC,EA
0766: 44 00 20    LXI     EA,$2000
0769: 74 e5       DSUB    EA,BC
076b: b5          DMOV    BC,EA
076c: 58 37       BIT     0,$FF37_switch2
076e: 14 ff 3f    LXI     BC,$3FFF
0771: 01 47       LDAW    $FF47_pwmDepth
0773: 48 2f       MUL     C
0775: 08          MOV     A,EAH
0776: 1b          MOV     C,A
0777: 01 47       LDAW    $FF47_pwmDepth
0779: 48 2e       MUL     B
077b: 70 43       EADD    EA,C
077d: b5          DMOV    BC,EA
077e: 44 ff 3f    LXI     EA,$3FFF
0781: 74 e5       DSUB    EA,BC
0783: 5e 1e       BIT     6,$FF1E_flags1     // square off/on
0785: 44 00 00    LXI     EA,$0000
0788: 48 93       STEAX   (HL)
078a: de          JR      $07A9
078b: 74 c5       DADD    EA,BC
078d: 08          MOV     A,EAH
078e: 47 e0       ONI     A,$E0
0790: 4f cd       JRE     $075F
0792: 44 ff 1f    LXI     EA,$1FFF
0795: 20 4a       INRW    $FF4A_lfoFlag
0797: 00          NOP     
0798: 4f c5       JRE     $075F
079a: 44 00 00    LXI     EA,$0000
079d: 20 4a       INRW    $FF4A_lfoFlag
079f: 00          NOP     
07a0: 4f bd       JRE     $075F
07a2: 14 00 20    LXI     BC,$2000
07a5: 74 c5       DADD    EA,BC
07a7: 4f c2       JRE     $076B
07a9: 34 39 ff    LXI     HL,$FF39_noiseLevel
07ac: 48 83       LDEAX   (HL)
07ae: 69 07       MVI     A,$07                  // noise VCA level
07b0: 78 2f       CALF    $082F_loadDac
07b2: 64 08 bf    ANI     PA,$BF                 // IC26 VCA CV mux
07b5: 54 ec 02    JMP     $02EC
07b8: 01 01       LDAW    $FF01_testMode
07ba: 37 01       LTI     A,$01
07bc: 54 80 02    JMP     $0280
07bf: 48 25       SLL     A
07c1: 48 a8       TABLE   
07c3: 21          JB      
07c4: c6 07       // only table entry is 07c6

which is right here, zero out the DAC for test mode
07c6: 78 51       CALF    $0851_resetBoard
07c8: 01 46       LDAW    $FF46_ic40Latch
07ca: 70 79 00 30 MOV     ($3000),A
07ce: 6b ef       MVI     C,$EF
07d0: 71 0f 00    MVIW    $FF0F_voicePtr,$00
07d3: 44 00 00    LXI     EA,$0000
07d6: 01 0f       LDAW    $FF0F_voicePtr
07d8: 78 2f       CALF    $082F_loadDac
07da: 0b          MOV     A,C
07db: 07 f0       ANI     A,$F0
07dd: 74 98 0f    ORAW    $FF0F_voicePtr
07e0: 4d c0       MOV     PA,A
07e2: 78 57       CALF    $0857_delay
07e4: 20 0f       INRW    $FF0F_voicePtr
07e6: 75 0f 08    EQIW    $FF0F_voicePtr,$08
07e9: e9          JR      $07D3
07ea: 48 27       SLL     C
07ec: 74 5b 80    OFFI    C,$80
07ef: 4f df       JRE     $07D0
07f1: 4f d3       JRE     $07C6
07f3: 00          NOP     
07f4: 00          NOP     
07f5: 00          NOP     
07f6: 00          NOP     
07f7: 00          NOP     
07f8: 00          NOP     
07f9: 00          NOP     
07fa: 00          NOP     
07fb: 00          NOP     
07fc: 00          NOP     
07fd: 00          NOP     
07fe: 00          NOP     
07ff: 00          NOP

handle ADC values
called with address of raw DAC values in HL
and "processed" values at DE
0800: 6b 03       MVI     C,$03      // four values
0802: 2b          LDAX    (HL)       // get raw value
0803: 70 b2       SUBNBX  (DE)       // subtract processed value
0805: 48 3a       NEGA               // if it borrows, negate A
0807: 27 01       GTI     A,$01      // is the difference between this time and last >1?
0809: d8          JR      $0822      // it is not
080a: 2b          LDAX    (HL)       // save the new value
080b: 3a          STAX    (DE)
080c: 36 04       SUINB   A,$04      // subtract 4 from what we just saved
080e: 69 00       MVI     A,$00      // 
0810: 1a          MOV     B,A
0811: 36 eb       SUINB   A,$EB
0813: c5          JR      $0819
0814: 60 a2       ADDNC   A,B
0816: 69 ff       MVI     A,$FF
0818: c1          JR      $081A
0819: 0a          MOV     A,B
081a: bb 08       STAX    (DE+$08)
081c: 22          INX     DE
081d: 32          INX     HL
081e: 53          DCR     C
081f: 4f e1       JRE     $0802
0821: b8          RET     
0822: 2a          LDAX    (DE)
0823: 4f e7       JRE     $080C
//
set 8253 mode word
called with the address of the command register in HL
0825: 69 36       MVI     A,$36  // mode word, read/load LSB first, BCD off, Mode 3
0827: 3b          STAX    (HL)
0828: 46 40       ADI     A,$40  // $76 = same for counter 1
082a: 3b          STAX    (HL)
082b: 46 40       ADI     A,$40  // $B6 = same for counter 2
082d: 3b          STAX    (HL)
082e: b8          RET     
//
set DAC value
called with the DAC channel to send in A
and the value to set it to in EA
082f: 64 18 f0    ORI     PA,$F0     // inhibit all MUXes
0832: 17 f0       ORI     A,$F0      // preset the MUX channel holding INH high
0834: 4d c0       MOV     PA,A
0836: 08          MOV     A,EAH      // upper byte to top six bits of DAC
0837: 4d c1       MOV     PB,A
0839: 09          MOV     A,EAL      // lower byte to bottom 6 bits of DAC
083a: 4d c2       MOV     PC,A       // lowest two bits not used for DAC
083c: b8          RET
//
envelope decay
called with the envelope level in BC
the high byte of the decay time in $FF35
and the low byte in A
083d: 48 2e       MUL     B              // high byte of value * low byte of coeff
083f: 08          MOV     A,EAH          // result in top byte of EA into A
0840: 1e          MOV     H,A            // save in H
0841: 01 35       LDAW    $FF35_dcyTemp  // high byte of value of coefficient
0843: 48 2f       MUL     C              // multiply by low byte of value
0845: 1f          MOV     L,A            // save A in L
0846: 08          MOV     A,EAH          // result into A
0847: 1b          MOV     C,A            // save that result in C
0848: 0f          MOV     A,L            // get the high byte back
0849: 48 2e       MUL     B              // high byte of value * high byte of coeff into EA
084b: 0e          MOV     A,H            // get the first value back
084c: 70 41       EADD    EA,A           // add that to EA
084e: 70 43       EADD    EA,C           // add the second result to ea
0850: b8          RET     // return with EA = (BC*decay) where decay is a binary fraction

test for "restart voice board" switch sent from assigner
0851: 5e 37       BIT     6,$FF37_switch2
0853: 54 80 02    JMP     $0280
0856: b8          RET

delay
uses very slow DIV instruction in a loop
0857: 6a 03       MVI     B,$03
0859: 44 10 00    LXI     EA,$0010
085c: 69 02       MVI     A,$02
085e: 48 3d       DIV     A
0860: 52          DCR     B
0861: f7          JR      $0859
0862: b8          RET  

masses of blank ROM from here up to the tables
0863: 00          NOP     
0864: 00          NOP     
0865: 00          NOP     
0866: 00          NOP     
0867: 00          NOP     
0868: 00          NOP     
0869: 00          NOP     
086a: 00          NOP     
086b: 00          NOP     
086c: 00          NOP     
086d: 00          NOP     
086e: 00          NOP     
086f: 00          NOP     
0870: 00          NOP     
0871: 00          NOP     
0872: 00          NOP     
0873: 00          NOP     
0874: 00          NOP     
0875: 00          NOP     
0876: 00          NOP     
0877: 00          NOP     
0878: 00          NOP     
0879: 00          NOP     
087a: 00          NOP     
087b: 00          NOP     
087c: 00          NOP     
087d: 00          NOP     
087e: 00          NOP     
087f: 00          NOP     
0880: 00          NOP     
0881: 00          NOP     
0882: 00          NOP     
0883: 00          NOP     
0884: 00          NOP     
0885: 00          NOP     
0886: 00          NOP     
0887: 00          NOP     
0888: 00          NOP     
0889: 00          NOP     
088a: 00          NOP     
088b: 00          NOP     
088c: 00          NOP     
088d: 00          NOP     
088e: 00          NOP     
088f: 00          NOP     
0890: 00          NOP     
0891: 00          NOP     
0892: 00          NOP     
0893: 00          NOP     
0894: 00          NOP     
0895: 00          NOP     
0896: 00          NOP     
0897: 00          NOP     
0898: 00          NOP     
0899: 00          NOP     
089a: 00          NOP     
089b: 00          NOP     
089c: 00          NOP     
089d: 00          NOP     
089e: 00          NOP     
089f: 00          NOP     
08a0: 00          NOP     
08a1: 00          NOP     
08a2: 00          NOP     
08a3: 00          NOP     
08a4: 00          NOP     
08a5: 00          NOP     
08a6: 00          NOP     
08a7: 00          NOP     
08a8: 00          NOP     
08a9: 00          NOP     
08aa: 00          NOP     
08ab: 00          NOP     
08ac: 00          NOP     
08ad: 00          NOP     
08ae: 00          NOP     
08af: 00          NOP     
08b0: 00          NOP     
08b1: 00          NOP     
08b2: 00          NOP     
08b3: 00          NOP     
08b4: 00          NOP     
08b5: 00          NOP     
08b6: 00          NOP     
08b7: 00          NOP     
08b8: 00          NOP     
08b9: 00          NOP     
08ba: 00          NOP     
08bb: 00          NOP     
08bc: 00          NOP     
08bd: 00          NOP     
08be: 00          NOP     
08bf: 00          NOP     
08c0: 00          NOP     
08c1: 00          NOP     
08c2: 00          NOP     
08c3: 00          NOP     
08c4: 00          NOP     
08c5: 00          NOP     
08c6: 00          NOP     
08c7: 00          NOP     
08c8: 00          NOP     
08c9: 00          NOP     
08ca: 00          NOP     
08cb: 00          NOP     
08cc: 00          NOP     
08cd: 00          NOP     
08ce: 00          NOP     
08cf: 00          NOP     
08d0: 00          NOP     
08d1: 00          NOP     
08d2: 00          NOP     
08d3: 00          NOP     
08d4: 00          NOP     
08d5: 00          NOP     
08d6: 00          NOP     
08d7: 00          NOP     
08d8: 00          NOP     
08d9: 00          NOP     
08da: 00          NOP     
08db: 00          NOP     
08dc: 00          NOP     
08dd: 00          NOP     
08de: 00          NOP     
08df: 00          NOP     
08e0: 00          NOP     
08e1: 00          NOP     
08e2: 00          NOP     
08e3: 00          NOP     
08e4: 00          NOP     
08e5: 00          NOP     
08e6: 00          NOP     
08e7: 00          NOP     
08e8: 00          NOP     
08e9: 00          NOP     
08ea: 00          NOP     
08eb: 00          NOP     
08ec: 00          NOP     
08ed: 00          NOP     
08ee: 00          NOP     
08ef: 00          NOP     
08f0: 00          NOP     
08f1: 00          NOP     
08f2: 00          NOP     
08f3: 00          NOP     
08f4: 00          NOP     
08f5: 00          NOP     
08f6: 00          NOP     
08f7: 00          NOP     
08f8: 00          NOP     
08f9: 00          NOP     
08fa: 00          NOP     
08fb: 00          NOP     
08fc: 00          NOP     
08fd: 00          NOP     
08fe: 00          NOP     
08ff: 00          NOP     
0900: 00          NOP     
0901: 00          NOP     
0902: 00          NOP     
0903: 00          NOP     
0904: 00          NOP     
0905: 00          NOP     
0906: 00          NOP     
0907: 00          NOP     
0908: 00          NOP     
0909: 00          NOP     
090a: 00          NOP     
090b: 00          NOP     
090c: 00          NOP     
090d: 00          NOP     
090e: 00          NOP     
090f: 00          NOP     
0910: 00          NOP     
0911: 00          NOP     
0912: 00          NOP     
0913: 00          NOP     
0914: 00          NOP     
0915: 00          NOP     
0916: 00          NOP     
0917: 00          NOP     
0918: 00          NOP     
0919: 00          NOP     
091a: 00          NOP     
091b: 00          NOP     
091c: 00          NOP     
091d: 00          NOP     
091e: 00          NOP     
091f: 00          NOP     
0920: 00          NOP     
0921: 00          NOP     
0922: 00          NOP     
0923: 00          NOP     
0924: 00          NOP     
0925: 00          NOP     
0926: 00          NOP     
0927: 00          NOP     
0928: 00          NOP     
0929: 00          NOP     
092a: 00          NOP     
092b: 00          NOP     
092c: 00          NOP     
092d: 00          NOP     
092e: 00          NOP     
092f: 00          NOP     
0930: 00          NOP     
0931: 00          NOP     
0932: 00          NOP     
0933: 00          NOP     
0934: 00          NOP     
0935: 00          NOP     
0936: 00          NOP     
0937: 00          NOP     
0938: 00          NOP     
0939: 00          NOP     
093a: 00          NOP     
093b: 00          NOP     
093c: 00          NOP     
093d: 00          NOP     
093e: 00          NOP     
093f: 00          NOP     
0940: 00          NOP     
0941: 00          NOP     
0942: 00          NOP     
0943: 00          NOP     
0944: 00          NOP     
0945: 00          NOP     
0946: 00          NOP     
0947: 00          NOP     
0948: 00          NOP     
0949: 00          NOP     
094a: 00          NOP     
094b: 00          NOP     
094c: 00          NOP     
094d: 00          NOP     
094e: 00          NOP     
094f: 00          NOP     
0950: 00          NOP     
0951: 00          NOP     
0952: 00          NOP     
0953: 00          NOP     
0954: 00          NOP     
0955: 00          NOP     
0956: 00          NOP     
0957: 00          NOP     
0958: 00          NOP     
0959: 00          NOP     
095a: 00          NOP     
095b: 00          NOP     
095c: 00          NOP     
095d: 00          NOP     
095e: 00          NOP     
095f: 00          NOP     
0960: 00          NOP     
0961: 00          NOP     
0962: 00          NOP     
0963: 00          NOP     
0964: 00          NOP     
0965: 00          NOP     
0966: 00          NOP     
0967: 00          NOP     
0968: 00          NOP     
0969: 00          NOP     
096a: 00          NOP     
096b: 00          NOP     
096c: 00          NOP     
096d: 00          NOP     
096e: 00          NOP     
096f: 00          NOP     
0970: 00          NOP     
0971: 00          NOP     
0972: 00          NOP     
0973: 00          NOP     
0974: 00          NOP     
0975: 00          NOP     
0976: 00          NOP     
0977: 00          NOP     
0978: 00          NOP     
0979: 00          NOP     
097a: 00          NOP     
097b: 00          NOP     
097c: 00          NOP     
097d: 00          NOP     
097e: 00          NOP     
097f: 00          NOP     
0980: 00          NOP     
0981: 00          NOP     
0982: 00          NOP     
0983: 00          NOP     
0984: 00          NOP     
0985: 00          NOP     
0986: 00          NOP     
0987: 00          NOP     
0988: 00          NOP     
0989: 00          NOP     
098a: 00          NOP     
098b: 00          NOP     
098c: 00          NOP     
098d: 00          NOP     
098e: 00          NOP     
098f: 00          NOP     
0990: 00          NOP     
0991: 00          NOP     
0992: 00          NOP     
0993: 00          NOP     
0994: 00          NOP     
0995: 00          NOP     
0996: 00          NOP     
0997: 00          NOP     
0998: 00          NOP     
0999: 00          NOP     
099a: 00          NOP     
099b: 00          NOP     
099c: 00          NOP     
099d: 00          NOP     
099e: 00          NOP     
099f: 00          NOP     
09a0: 00          NOP     
09a1: 00          NOP     
09a2: 00          NOP     
09a3: 00          NOP     
09a4: 00          NOP     
09a5: 00          NOP     
09a6: 00          NOP     
09a7: 00          NOP     
09a8: 00          NOP     
09a9: 00          NOP     
09aa: 00          NOP     
09ab: 00          NOP     
09ac: 00          NOP     
09ad: 00          NOP     
09ae: 00          NOP     
09af: 00          NOP     
09b0: 00          NOP     
09b1: 00          NOP     
09b2: 00          NOP     
09b3: 00          NOP     
09b4: 00          NOP     
09b5: 00          NOP     
09b6: 00          NOP     
09b7: 00          NOP     
09b8: 00          NOP     
09b9: 00          NOP     
09ba: 00          NOP     
09bb: 00          NOP     
09bc: 00          NOP     
09bd: 00          NOP     
09be: 00          NOP     
09bf: 00          NOP     
09c0: 00          NOP     
09c1: 00          NOP     
09c2: 00          NOP     
09c3: 00          NOP     
09c4: 00          NOP     
09c5: 00          NOP     
09c6: 00          NOP     
09c7: 00          NOP     
09c8: 00          NOP     
09c9: 00          NOP     
09ca: 00          NOP     
09cb: 00          NOP     
09cc: 00          NOP     
09cd: 00          NOP     
09ce: 00          NOP     
09cf: 00          NOP     
09d0: 00          NOP     
09d1: 00          NOP     
09d2: 00          NOP     
09d3: 00          NOP     
09d4: 00          NOP     
09d5: 00          NOP     
09d6: 00          NOP     
09d7: 00          NOP     
09d8: 00          NOP     
09d9: 00          NOP     
09da: 00          NOP     
09db: 00          NOP     
09dc: 00          NOP     
09dd: 00          NOP     
09de: 00          NOP     
09df: 00          NOP     
09e0: 00          NOP     
09e1: 00          NOP     
09e2: 00          NOP     
09e3: 00          NOP     
09e4: 00          NOP     
09e5: 00          NOP     
09e6: 00          NOP     
09e7: 00          NOP     
09e8: 00          NOP     
09e9: 00          NOP     
09ea: 00          NOP     
09eb: 00          NOP     
09ec: 00          NOP     
09ed: 00          NOP     
09ee: 00          NOP     
09ef: 00          NOP     
09f0: 00          NOP     
09f1: 00          NOP     
09f2: 00          NOP     
09f3: 00          NOP     
09f4: 00          NOP     
09f5: 00          NOP     
09f6: 00          NOP     
09f7: 00          NOP     
09f8: 00          NOP     
09f9: 00          NOP     
09fa: 00          NOP     
09fb: 00          NOP     
09fc: 00          NOP     
09fd: 00          NOP     
09fe: 00          NOP     
09ff: 00          NOP    
//
Lookup tables for the various controls

0A00_portCoeffTbl:
0a00: 00 ff f7 ef e7 df d7 cf c7 bf b7 af a7 9f 97 8f
0a10: 87 7f 77 6f 67 5f 57 4f 47 3f 3d 3b 39 37 35 33
0a20: 31 2f 2d 2b 29 27 25 23 21 1f 1d 1b 19 17 15 13
0a30: 12 11 10 10 10 10 0f 0f 0f 0f 0e 0e 0e 0e 0d 0d
0a40: 0d 0d 0d 0c 0c 0c 0c 0c 0b 0b 0b 0b 0b 0a 0a 0a
0a50: 0a 0a 09 09 09 09 09 08 08 08 08 08 07 07 07 07
0a60: 07 06 06 06 06 06 05 05 05 05 05 04 04 04 04 04
0a70: 03 03 03 03 03 02 02 02 02 02 01 01 01 01 01 01
0a80: 00 00 00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d
0a90: 0e 0f 10 11 12 13 14 15 16 17 18 19 1a 1b 1c 1d
0aa0: 1e 1f 20 21 22 23 24 25 26 27 28 29 2a 2b 2c 2d
0ab0: 2e 2f 30 31 32 33 34 35 36 37 38 39 3a 3b 3c 3d
0ac0: 3e 40 42 44 46 48 4a 4c 4e 50 52 54 56 58 5a 5c
0ad0: 5e 60 62 64 66 68 6a 6c 6e 70 72 74 76 78 7a 7c
0ae0: 80 84 88 8c 90 94 98 9c a0 a4 a8 ac b0 b4 b8 bc
0af0: c0 c4 c8 cc d0 d4 d8 dc e0 e4 e8 ec f0 f8 ff ff

not used, 0x30 bytes of 0 
0b00: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
0b10: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
0b20: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
//
0B30_lfoDelayTbl1:
0b30: ffff 0419 020c 015e 0100 0100 0100 0100

voice to clock address lookup table
0b50: 20 21 22 10 11 12

CTC programming command for clock chip
0b56: 36 76 b6 36 76 b6

0b5c: 00 00 00 00


0b60_envAtkTbl:
0b60: 4000 2000 1000 0aaa 0800 0666 0555 0492
0b70: 0400 038e 0333 02e9 02ab 0276 0249 0222
0b80: 0200 01e2 01c7 01af 0199 0186 0174 0164
0b90: 0155 0148 013b 012f 0124 011a 0111 0108
0ba0: 0100 00f8 00f1 00ea 00e4 00dd 00d8 00d2
0bb0: 00cd 00c8 00c3 00bf 00ba 00b6 00b2 00ae
0bc0: 00ab 00a7 00a4 00a1 009e 009b 0098 0095
0bd0: 0092 0090 008d 008b 0089 0086 0084 0082
0be0: 007f 007d 007a 0077 0074 0072 006f 006c
0bf0: 0069 0067 0064 0061 005e 005c 0059 0056
0c00: 0053 0050 004e 004b 0048 0045 0042 0040
0c10: 003f 003d 003c 003a 0039 0037 0036 0034
0c20: 0033 0031 0030 002e 002d 002b 002a 0028
0c30: 0027 0025 0024 0022 0021 0021 0020 0020
0c40: 001f 001f 001e 001e 001d 001d 001c 001c
0c50: 001b 001b 001a 0019 0018 0017 0016 0015

0c60_lfoSpeedTbl:
0c60: 0005 000f 0019 0028 0037 0046 0050 005a
0c70: 0064 006e 0078 0082 008c 0096 00a0 00aa
0c80: 00b4 00be 00c8 00d2 00dc 00e6 00f0 00fa
0c90: 0104 010e 0118 0122 012c 0136 0140 014a
0ca0: 0154 015e 0168 0172 017c 0186 0190 019a
0cb0: 01a4 01ae 01b8 01c2 01cc 01d6 01e0 01ea
0cc0: 01f4 01fe 0208 0212 021c 0226 0230 023a
0cd0: 0244 024e 0258 0262 026c 0276 0280 028a
0ce0: 029a 02aa 02ba 02ca 02da 02ea 02fa 030a
0cf0: 031a 032a 033a 034a 035a 036a 037a 038a
0d00: 039a 03aa 03ba 03ca 03da 03ea 03fa 040a
0d10: 041a 042a 043a 044a 045a 046a 047a 048a
0d20: 04be 04f2 0526 055a 058e 05c2 05f6 062c
0d30: 0672 06b8 0708 0758 07a8 07f8 085c 08c0
0d40: 0924 0988 09ec 0a50 0ab4 0b18 0b7c 0be0
0d50: 0c58 0cd0 0d48 0dde 0e74 0f0a 0fa0 1000
0d60: 1000 3000 5000 7000 9000 a000 a800 b000

0D60_envDecRelTbl:
0d70: b800 c000 c800 d000 d800 e000 e800 f000
0d80: f080 f100 f180 f200 f280 f300 f380 f400
0d90: f480 f500 f580 f600 f680 f700 f780 f800
0da0: f880 f900 f980 fa00 fa80 fb00 fb80 fc00
0db0: fc80 fd00 fd80 fe00 fe0c fe18 fe24 fe30
0dc0: fe3c fe48 fe54 fe60 fe6c fe78 fe84 fe90
0dd0: fe9c fea8 feb4 fec0 fecc fed8 fee4 fef0
0de0: fefc ff08 ff0c ff10 ff14 ff18 ff1c ff20
0df0: ff24 ff28 ff2c ff30 ff34 ff38 ff3c ff40
0e00: ff44 ff48 ff4c ff50 ff54 ff58 ff5c ff60
0e10: ff64 ff68 FF6C ff70 ff74 ff78 ff7c ff80
0e20: ff84 ff88 ff8c ff90 ff94 ff98 ff9c ffa0
0e30: ffa4 ffa8 ffac ffb0 ffb4 ffb8 ffbc ffc0
0e40: ffc4 ffc8 ffcc ffd0 ffd4 ffd8 ffdc ffe0
0e50: ffe4 ffe8 ffec fff0 fff1 fff2 fff3 fff4

0E60_noteCvDacTbl:
0e60: 0020 0022 0024 0026 0028 002b 002d 0030
0e70: 0033 0036 0039 003c 0040 0044 0048 004c
0e80: 0050 0055 005a 005f 0065 006b 0072 0079
0e90: 0080 0087 008f 0098 00a1 00ab 00b5 00c0
0ea0: 00cb 00d7 00e4 00f2 0100 010f 011f 0130
0eb0: 0142 0156 016a 017f 0196 01ae 01c8 01e3
0ec0: 0200 021e 023e 0261 0285 02ab 02d4 02ff
0ed0: 032d 035d 0390 03c7 0400 043d 047d 04c2
0ee0: 050a 0557 05a9 05fe 0659 06ba 0721 078d
0ef0: 0800 0879 08fb 0984 0a15 0aae 0b51 0bfc
0f00: 0cb2 0d74 0e42 0f1b 0fff 0fff 0fff 0fff
0f10: 0fff 0fff 0fff 0fff 0fff 0fff 0fff 0fff
0f20: 0fff 0fff 0fff 0fff 0fff 0fff 0fff 0fff

0F30_noteClkDivTbl:
0f30: f06e e2e9 d61f ca18 beba b404 a9e0 a052
0f40: 974f 8ecc 86c5 7f33 780f 7151 6aef 64ee
0f50: 5f42 59e9 54d9 5015 4b95 4754 4353 3f8d
0f60: 3bfc 389e 356e 326e 2f99 2ced 2a66 2805
0f70: 25c5 23a5 21a5 1fc3 1dfb 1c4c 1ab4 1934
0f80: 17ca 1674 1531 1401 12e1 11d1 10d1 0fe1
0f90: 0efd 0e25 0d59 0c99 0be4 0b39 0a98 0a00
0fa0: 0970 08e8 0868 07f0 077e 0712 06ac 064c
0fb0: 05f2 059c 054c 0500 04b8 0474 0434 03f8
0fc0: 03bf 0389 0356 0326 02f9 02ce 02a6 0280
0fd0: 025c 023a 021a 01fc 01df 01c4 01ab 0193
0fe0: 017c 0167 0153 0140 012e 011d 010d 00fe
0ff0: 00f0 00e2 00d5 00c9 00be 00b3 00a9 00a0
