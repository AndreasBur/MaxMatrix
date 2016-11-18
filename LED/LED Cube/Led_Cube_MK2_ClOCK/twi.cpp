#include "twi.h"
#include "animation.h"
#include <compat/twi.h>

#define BUFFER_LENGTH 32

uint8_t rxBufferIndex = 0;
uint8_t rxBuffer[BUFFER_LENGTH];

uint8_t txAddress = 0;
uint8_t txBufferIndex = 0;
uint8_t txBufferLength = 0;
uint8_t txBuffer[BUFFER_LENGTH];

twi::twi(void){
}

void twi::begin(void){
	SB(PORTC, 5);
	SB(PORTC, 4);

	CB(TWSR, TWPS0);
	CB(TWSR, TWPS1);
	
	TWBR = 0x48;
	
	TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWEA);
}

void twi::beginTransmission(uint8_t address){
	txAddress = address;
	txBufferIndex = 0;
	txBufferLength = 0;
}

void twi::write(uint8_t data){
	txBuffer[txBufferLength] = data;
	txBufferLength++;
}

uint8_t twi::endTransmission(void){
	txBufferIndex  = 0;

	//SEND START CONDITION
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	
	while(!(TWCR & (1<<TWINT)));
	
	if((TWSR & 0xF8) != TW_START){
		return '1';
	}
	
	//SEND ADDRESS
	TWDR = TW_WRITE;
	TWDR |= txAddress<<1;
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	
	while(!(TWCR & (1<<TWINT)));
	
	if((TWSR & 0xF8) != TW_MT_SLA_ACK){
		return '2';
	}
	
	//SEND ALL THE DATA IN THE BUFFER
	while(txBufferIndex < txBufferLength){
		TWDR = txBuffer[txBufferIndex];
		
		txBufferIndex++;
		
		TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
		
		while(!(TWCR & (1<<TWINT)));
		
		if((TWSR & 0xF8) != TW_MT_DATA_ACK){
			return '3';
		}
	}
	
	//SEND STOP CONDITION
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	
	txBufferIndex = 0;
	txBufferLength = 0;
	
	return 0;
}

uint8_t twi::requestFrom(uint8_t address, uint8_t length){
	uint8_t err = 0;
	uint16_t c = 0;
	rxBufferIndex  = 0;
	
	//SEND START CONDITION
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	
	while(!(TWCR & (1<<TWINT)));
	
	if((TWSR & 0xF8) != TW_REP_START){
		return '4';
	}
	
	TWDR = TW_READ|(address<<1);
	
	TWCR = (1<<TWEN)|(1<<TWINT);
	while(!(TWCR & (1<<TWINT)));
	
	while(length > 0){
		if(length == 1){
			TWCR = (1<<TWEN)|(1<<TWINT);
		}else{
			TWCR = (1<<TWEN)|(1<<TWINT)|(1<<TWEA);
		}
		
		while(!(TWCR & (1<<TWINT)));
		
		switch(TWSR & 0xF8){
			case TW_MR_DATA_ACK:
				rxBuffer[rxBufferIndex] = TWDR;
				rxBufferIndex++;
				length--;
				err = '5';
				break;
			case TW_MR_SLA_ACK:
				err = '6';
				break;
			case TW_MR_DATA_NACK:
				rxBuffer[rxBufferIndex] = TWDR;
				length = 0;
				err = '7';
				break;
			case TW_MR_SLA_NACK:
				err = '8';
				break;
		}
		
		c++;
		
		if(c > 1000){
			return err;
		}
	}
	
	//SEND STOP CONDITION
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	
	rxBufferIndex  = 0;
	
	return 0;
}

uint8_t twi::receive(void){
	uint8_t ret = 0;
	
	ret = rxBuffer[rxBufferIndex];
	rxBufferIndex++;
	
	return ret;
}