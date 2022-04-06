PROCESSOR 18F8722
    
#include <xc.inc>

; configurations
CONFIG OSC = HSPLL, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF, WDT = OFF, MCLRE = ON, LPT1OSC = OFF, LVP = OFF, XINST = OFF, DEBUG = OFF

; global variable declarations
GLOBAL var1,var2,var3,pB,pC,pD,selectedPort,isRA4Pressed, isRE4Pressed

; allocating memory for variables
PSECT udata_acs
    var1:
	DS     1    ; allocates 1 byte
    var2:
	DS     1
    var3:
	DS     1
    pB:
	DS     1    ; allocates 1 byte
    pC:
	DS     1
    pD:
	DS     1
    selectedPort:
	DS     1
    isRA4Pressed:
	DS     1
    isRE4Pressed:
	DS     1

PSECT resetVec,class=CODE,reloc=2
resetVec:
    goto    main

; DO NOT DELETE OR MODIFY
; 500ms pass check for test scripts
ms500_passed:
    nop
    return

; DO NOT DELETE OR MODIFY
; 1sec pass check for test scripts
ms1000_passed:
    nop
    return

stale4ms:
    clrf var1
    clrf var2
first_loop:
    incf var1
    movf var1,W
    xorlw 255
    bz second_loop
    goto first_loop
second_loop:
    clrf var1
    incf var2
    movf var2,W
    xorlw 25
    bz return_stale
    goto first_loop
return_stale:
    clrf var1
    clrf var2
    return
    
delay1000ms:
    call stale4ms
    incf var3
    movf var3,W
    xorlw 255
    bz delay1000ms_return
    goto delay1000ms
delay1000ms_return:
    clrf var3
    return
    
delay500ms:
    call stale4ms
    incf var3
    movf var3,W
    xorlw 64
    bz delay500ms_return
    goto delay500ms
delay500ms_return:
    clrf var3
    return
    
increase_port:
    movlw 0x01
    addwf selectedPort
    movlw 0x03
    andwf selectedPort
    return
    
toggle_portC:
    movlw 0xFF
    andwf pC, 0
    bz turnOff_C
    ;turnOn_C
    movff pC, PORTC
    clrf pC
    return
turnOff_C:
    movff PORTC, pC
    clrf PORTC
    return

toggle_portB:
    movlw 0xFF
    andwf pB, 0
    bz turnOff_B
    ;turnOn_B
    movff pB, PORTB
    clrf pB
    return
turnOff_B:
    movff PORTB, pB
    clrf PORTB
    return
    
toggle_portD:
    movlw 0x01 
    subwf pD, 0
    bz first_it_portD
    movlw 0xFE
    andwf PORTD
    RRNCF PORTD
    return
first_it_portD:
    
    return
change_portC:
    
    
ra4_action:
    movlw 0x01
    andwf selectedPort, 0
    bnz one_or_three_ra4
    ; zero or two
    movlw 0x02
    andwf selectedPort, 0
    bnz selectedPortC_ra4
    ; selectedPort = 0
    goto loop
one_or_three_ra4:
    movlw 0x02
    andwf selectedPort, 0
    bz selectedPortB_ra4
    ; selectedPortD
    goto loop
selectedPortC_ra4:
    movlw 0xFF
    andwf PORTC,0
    bz portC_blink
    movlw 0x03
    xorwf PORTC
    goto loop
portC_blink:
    movlw 0x03
    xorwf pC
    goto loop
selectedPortB_ra4:
    movlw 0xFF
    andwf PORTB,0
    bz portB_blink
portB_blink_return:
    movlw 0x01
    subwf var1, 0
    bz portB_1
    movlw 0x03
    subwf var1, 0
    bz portB_3
    movlw 0x07
    subwf var1, 0
    bz portB_7
    movlw 0x0F
    subwf var1, 0
    bz portB_15
    ; blink press
portB_swap:
    movlw 0xFF
    andwf PORTB,0
    bz portB_swap_pB
    movff var1, PORTB
    goto loop
portB_swap_pB:
    movff var1, pB
    goto loop
    
portB_blink:
    movff pB, var1
    goto portB_blink_return
portB_1:
    movlw 0x03
    movwf var1
    goto portB_swap
portB_3:
    movlw 0x07
    movwf var1
    goto portB_swap
portB_7:
    movlw 0x0F
    movwf var1
    goto portB_swap
portB_15:
    movlw 0x01
    movwf var1
    goto portB_swap

    
	
PSECT CODE
main:
    ; some code to initialize and wait 1000ms here, maybe
    movlw 0
    movwf var1
    movwf selectedPort
    MOVLW 0x00                         ; all output
    MOVWF TRISB
    CLRF PORTB                         ; clear all
    MOVWF TRISC
    CLRF PORTC                         ; clear all
    MOVWF TRISD
    CLRF PORTD                         ; clear all
    MOVLW 0x10                         ; pin4 input, others output
    MOVWF TRISA
    CLRF PORTA
    MOVWF TRISE
    CLRF PORTE
    movlw 0x0F
    movwf PORTB
    movlw 0x03
    movwf PORTC
    movlw 0xFF
    movwf PORTD
    call delay1000ms
    call ms1000_passed
    movlw 0x01
    movwf PORTB
    movwf PORTC
    CLRF PORTD
    ; a loop here, maybe
    loop:
	incf var3
	call stale4ms
	movf var3,W
	xorlw 130
	bz wait_passed
        ; inside the loop, once it is confirmed 500ms passed
	movlw 0x10 ; check if ra4 pressed
	movwf var1
	movf PORTA,W
	andwf var1
	bz ra4_not_pressed ; not pressed branch
	movlw 0x01; ra4 pressed branch
	movwf isRA4Pressed
	goto loop
	movlw 0x10 ; check if re4 pressed
	movwf var1
	movf PORTE,W
	andwf var1
	bz re4_not_pressed
	;re4 pressed branch
	movlw 0x01
	movwf isRE4Pressed
	goto loop
    re4_not_pressed:
	movlw 0x01
	andwf isRE4Pressed
	bnz re4_released
	goto loop
    re4_released:
    	clrf isRE4Pressed
	call increase_port
	goto loop
    ra4_released:
	clrf isRA4Pressed
	; increase portb - 1, portc - 2, portd - 3 : selectedport
	; missing function , fix later
	goto ra4_action
    ra4_not_pressed:
	movlw 0x01
	andwf isRA4Pressed
	bnz ra4_released
	;call increase_selectedPort
	goto loop
    wait_passed:
        call ms500_passed
	clrf var3
	movlw 0x01
	andwf selectedPort, 0
	bnz one_or_three
	; zero or two
	movlw 0x02
	andwf selectedPort, 0
	bnz selectedPortC
	; selectedPort = 0
	goto loop
    one_or_three:
	movlw 0x02
	andwf selectedPort, 0
	bz selectedPortB
	; selectedPortD
	movlw 0x01
	addwf pD
	call toggle_portD
	goto loop
    selectedPortC:
	call toggle_portC
	goto loop
    selectedPortB:
	call toggle_portB
        goto loop

end resetVec