// Numeros padronizados

#include <bits/stdint-uintn.h>
#include <bits/types/FILE.h>
#include <stdlib.h>
#include <cinttypes>
#include <cmath>
#include <cstdio>
#include <string>

std::string specifyRegL(uint8_t value) {
	if (value == 28) {
		return "ir";
	} else if (value == 29) {
		return "pc";
	} else if (value == 30) {
		return "sp";
	} else if (value == 31) {
		return "sr";
	} else {
		return "r" + std::to_string(value);
	}
}

std::string specifyRegU(uint8_t value) {
	if (value == 28) {
		return "IR";
	} else if (value == 29) {
		return "PC";
	} else if (value == 30) {
		return "SP";
	} else if (value == 31) {
		return "SR";
	} else {
		return "R" + std::to_string(value);
	}
}

uint32_t setRegister(uint32_t r, uint32_t value) {
	if (r == 0) {
		return 0;
	} else {
		return value;
	}
}

void verifyZero(uint32_t r, uint32_t part, uint64_t &temp) {
	if (r == 0) {
		if (part == 1) {
			temp = temp & 0xFFFFFFFF;
		} else {
			temp = temp & 0xFFFFFFFF00000000;
		}
	}
}

bool isSpecificReg(uint32_t r) {
	return (r == 28 || r == 29 || r == 30 || r == 31);
}

bool checkBit(uint64_t R, int bitPosition) {
	uint64_t mask = static_cast<uint64_t>(1u) << bitPosition;
	return (R & mask) != 0;
}

bool checkBit(uint32_t R, int bitPosition) {
	uint32_t mask = static_cast<uint32_t>(1u) << bitPosition;
	return (R & mask) != 0;
}

bool checkBit(uint16_t R, int bitPosition) {
	uint16_t mask = static_cast<uint16_t>(1u) << bitPosition;
	return (R & mask) != 0;
}

uint32_t signExtension32(uint16_t i) {
	bool isOne = ((static_cast<uint16_t>(i) >> 15) & 1) == 1;
	uint32_t answer = 0;

	if (isOne) {
		answer = static_cast<uint32_t>(i) | 0xFFFF0000;
	} else {
		answer = static_cast<uint32_t>(i);
	}

	return answer;
}

uint32_t extend26To32(uint32_t i) {
	uint32_t answer = 0;
	if (checkBit(i, 25)) {
		answer = static_cast<uint32_t>(i) | 0b11111100000000000000000000000000;
	} else {
		answer = static_cast<uint32_t>(i);
	}

	return answer;
}

uint64_t extend64To64(uint64_t i, bool value) {
	uint64_t answer = 0;
	if (value) {
		answer = i | 0xFFFFFFFF00000000;
	} else {
		answer = i;
	}
	return answer;
}

uint32_t extend16To32(uint32_t i) {
	uint32_t answer = 0;
	if (checkBit(i, 15)) {
		answer = static_cast<uint32_t>(i) | 0b11111111111111110000000000000000;
	} else {
		answer = static_cast<uint32_t>(i);
	}

	return answer;
}

uint32_t extend21To32(uint32_t i) {
	uint32_t answer = 0;
	if (checkBit(i, 20)) {
		answer = static_cast<uint32_t>(i) | 0b11111111111000000000000000000000;
	} else {
		answer = static_cast<uint32_t>(i);
	}

//	// Convert the number to a bit representation using std::bitset
//	std::bitset<32> bitRepresentation(answer);
//
//	// Print the original number and its bit representation
//	std::cout << "Number: " << answer << std::endl;
//	std::cout << "Bit Representation: " << bitRepresentation << std::endl;

	return answer;
}

uint64_t extend32To64(uint32_t i) {
	uint64_t answer = 0;
	if (checkBit(i, 31)) {
		answer = static_cast<uint64_t>(i) | 0xFFFFFFFF00000000;
		;
	} else {
		answer = static_cast<uint64_t>(i);
	}
	return answer;
}

uint32_t extend32To32(uint32_t i, bool value) {
	uint32_t answer = 0;
	if (!value) {
		answer = ~i;
		answer = i | answer;
	} else {
		answer = i;
	}
	return answer;
}

uint64_t toTwoComplement64bits(uint32_t r) {
	uint64_t newValue = 0;
	newValue = extend32To64(r);
	newValue = ~newValue;
	newValue = newValue + 1;
	return newValue;
}

uint32_t toTwoComplement32bits(uint32_t r) {
	uint32_t value = 0;
	value = r;
	value = ~value;
	value = value + 1;
	return value;
}

uint32_t toTwoComplement32bitsNegative(uint32_t r, bool sign) {
	uint32_t value = 0;
	value = r;
	if (!sign) {
		value = ~value;
		value = value + 1;
	}
	return value;
}

uint32_t toTwoComplement32bitsIfNegative(uint32_t r) {
	uint32_t value = 0;
	value = r;
	if (checkBit(r, 31)) {
		value = ~value;
		value = value + 1;
	}
	return value;
}

bool signResult(uint32_t a, uint32_t b) {

	if (a == 0 && b == 0) {
		return true;
	}

	if (a == 0 || b == 0) {
		if (a == 0) {
			return !checkBit(b, 31);
		} else {
			return !checkBit(a, 31);
		}
	}

	if (checkBit(a, 31) && checkBit(b, 31)) {
		return true;
	} else {
		return false;
	}
}

// possible wrong
bool signResultMod(uint32_t a, uint32_t b) {
	if (checkBit(a, 31) && checkBit(b, 31)) {
		return false;
	} else if (checkBit(a, 31)) {
		return false;
	} else {
		return true;
	}
}

// ZN bit 6, zero
void setZN(uint32_t &SR, bool value) {
	// if Value is true then set the 6th bit to 0

	if (value) {
		SR = SR | static_cast<uint32_t>(0b1000000);
	} else {
		SR = SR & ~(static_cast<uint32_t>(0b1000000));
	}
}

// ZD bit 5, division by zero
void setZD(uint32_t &SR, bool value) {
	// Change bit 5 to 1 if value is true

	if (value) {
		// SR | ((0b100000 << 26) >>> 26)
		SR = SR | static_cast<uint32_t>(0b100000);
	} else {
		SR = SR & ~(static_cast<uint32_t>(0b100000));
	}
}

// SN bit 4, sign
void setSN(uint32_t &SR, bool value) {
	// Change bit 4 to 1 if value is true

	if (value) {
		// Setting to one, then is positive
		SR = SR | static_cast<uint32_t>(0b10000);
	} else {
		// setting to zero, then is negative
		SR = SR & ~(static_cast<uint32_t>(0b10000));
	}
}

// OV bit 3, overflow
void setOV(uint32_t &SR, bool value) {
	// Change bit 3 to 1 if value is true

	if (value) {
		// SR | ((0b1000 << 28) >>> 28)
		SR = SR | static_cast<uint32_t>(0b1000);
	} else {

		SR = SR & ~(static_cast<uint32_t>(0b1000));
	}
}

// IV bit 2, invalid instruction
void setIV(uint32_t &SR, bool value) {
	// Change bit 2 to 1 if value is true

	if (value) {
		// SR | ((0b100 << 29) >>> 29)
		SR = SR | static_cast<uint32_t>(0b100);
	} else {
		SR = SR & ~(static_cast<uint32_t>(0b100));
	}
}

// CY bit 0, carry
void setCY(uint32_t &SR, bool value) {
	// Change bit 0 to 1 if value is true

	if (value) {
		// SR | ((0b1 << 31) >>> 31)
		SR = SR | (static_cast<uint32_t>(0b1));
	} else {
		SR = SR & ~(static_cast<uint32_t>(0b1));
	}
}

// ZN 6
bool getZN(uint32_t R) {
	// if checkBit is 0 then it is true
	return checkBit(R, 6);
}

// ZD 5
bool getZD(uint32_t R) {
	return checkBit(R, 5);
}

// SN 4
bool getSN(uint32_t R) {
	return checkBit(R, 4);
}

// OV 3
bool getOV(uint32_t R) {
	return checkBit(R, 3);
}

// IV 2
bool getIV(uint32_t R) {
	return checkBit(R, 2);
}

// CY 0
bool getCY(uint32_t R) {
	return checkBit(R, 0);
}

int main(int argc, char *argv[]) {

	FILE *read = fopen(argv[1], "r");
	char buffer[255];

	uint32_t R[32] = { 0 };
	uint32_t *MEM32 = (uint32_t*) (calloc(8, 1024));

	int j = 0;
	while (fgets(buffer, 255, read) != NULL) {
		uint32_t value = strtoul(buffer, NULL, 16);
		MEM32[j] = value;
		j++;
	}

	fclose(read);

	FILE *write = fopen(argv[2], "w");

	fprintf(write, "[START OF SIMULATION]\n");

	char instrucao[30] = { 0 };

	uint8_t z = 0;
	uint8_t x = 0;
	uint8_t y = 0;
	uint16_t l = 0;

	uint8_t v = 0;
	uint8_t w = 0;

	uint16_t i16bits = 0;
	uint32_t i32bits = 0;

	uint32_t pc = 0;
	uint32_t xyl = 0;
	uint32_t sp = 0;

	//  cmpi
	uint64_t CMPI = 0;
	bool Rx_31 = false;
	bool i_15 = false;
	bool cmpi_31 = false;

	// Sub
	uint64_t rz = 0;

	// subi
	uint64_t Rz_subi = 0;
	bool Rx_31_subi = false;
	bool i_15_subi = false;
	bool Rz_31_subi = false;

	// mul

	uint8_t multOperand = 0;

	// 0b000100
	uint64_t RlxRz_mul1 = 0;
	uint32_t Rl_mul1 = 0;
	uint32_t Rz_mul1 = 0;

	// call 1
	uint32_t bitmask = 0;

	uint64_t temp64_1 = 0;
	uint64_t temp64_2 = 0;
	uint64_t temp64_3 = 0;

	uint32_t temp32_1 = 0;
	uint32_t temp32_2 = 0;
	uint32_t temp32_3 = 0;

	uint64_t tmp64Mul_1;
	uint64_t tmp64Mul_2;

	bool tempB_1 = 0;
	bool tempB_2 = 0;
	bool tempB_3 = 0;

	bool execution = true;
	int i = 0;
	int tmpCount = 0;
	while (execution) {

		R[28] = MEM32[R[29] >> 2];

		uint32_t operation = (R[28] & (0b111111 << 26)) >> 26;

		switch (operation) {

		// mov
		case 0b000000:
			// 0x????????:	mov rz,u                 	Rz=0x?????????

			z = (R[28] & (0b11111 << 21)) >> 21;
			xyl = R[28] & 0x1FFFFF;
			R[z] = xyl;

			sprintf(instrucao, "mov %s,%u", specifyRegL(z).c_str(), xyl);
			// Formatacao de saida em tela (deve mudar para o arquivo de saida)
			fprintf(write, "0x%08X:\t%-25s\t%s=0x%08X\n", R[29], instrucao,
					specifyRegU(z).c_str(), xyl);

			break;

			// movs
		case 0b000001:

			z = (R[28] & (0b11111 << 21)) >> 21;
			xyl = R[28] & 0x1FFFFF;
			R[z] = extend21To32(xyl);

			//			0x????????:	movs rz,s Rz=0x????????
			if (isSpecificReg(z)) {
				sprintf(instrucao, "movs %s,%i", specifyRegU(z).c_str(),
						extend21To32(xyl));
				// Formatacao de saida em tela (deve mudar para o arquivo de saida)
				fprintf(write, "0x%08X:\t%-25s\t%s=0x%08X\n", R[29], instrucao,
						specifyRegU(z).c_str(), extend21To32(xyl));
			} else {
				sprintf(instrucao, "movs r%u,%i", z, extend21To32(xyl));
				// Formatacao de saida em tela (deve mudar para o arquivo de saida)
				fprintf(write, "0x%08X:\t%-25s\tR%u=0x%08X\n", R[29], instrucao,
						z, extend21To32(xyl));
			}

			break;

			// add
		case 0b000010:

			pc = R[29];
			x = (R[28] & (0b11111 << 16)) >> 16;
			y = (R[28] & (0b11111 << 11)) >> 11;
			z = (R[28] & (0b11111 << 21)) >> 21;

			temp64_1 = static_cast<uint64_t>(R[x])
					+ static_cast<uint64_t>(R[y]);
			R[z] = temp64_1;

			if (temp64_1 == 0) {
				setZN(R[31], true);
			} else {
				setZN(R[31], false);
			}

			if (checkBit(temp64_1, 31)) {
				setSN(R[31], true);
			} else {
				setSN(R[31], false);
			}

			tempB_1 = checkBit(R[x], 31);
			tempB_2 = checkBit(R[y], 31);
			tempB_3 = checkBit(R[z], 31);

			if ((tempB_1 == tempB_2) && (tempB_3 != tempB_1)) {
				setOV(R[31], true);
			} else {
				setOV(R[31], false);
			}

			if (checkBit(temp64_1, 32)) {
				setCY(R[31], true);
			} else {
				setCY(R[31], false);
			}

//			0x????????:	add rz,rx,ir             	Rz=Rx+IR=0x????????,SR=0x????????
			sprintf(instrucao, "add %s,%s,%s", specifyRegL(z).c_str(),
					specifyRegL(x).c_str(), specifyRegL(y).c_str());
			// Formatacao de saida em tela (deve mudar para o arquivo de saida)
			fprintf(write, "0x%08X:\t%-25s\t%s=%s+%s=0x%08X,SR=0x%08X\n", pc,
					instrucao, specifyRegU(z).c_str(), specifyRegU(x).c_str(),
					specifyRegU(y).c_str(), R[z], R[31]);

			break;

			// sub
		case 0b000011:

			pc = R[29];
			x = (R[28] & (0b11111 << 16)) >> 16;
			z = (R[28] & (0b11111 << 21)) >> 21;
			y = (R[28] & (0b11111 << 11)) >> 11;

			temp64_1 = static_cast<uint64_t>(R[x])
					- static_cast<uint64_t>(R[y]);
			R[z] = setRegister(z, temp64_1);

			if (R[z] == 0) {
				setZN(R[31], true);
			} else {
				setZN(R[31], false);
			}

			if (checkBit(R[z], 31)) {
				setSN(R[31], true);
			} else {
				setSN(R[31], false);
			}

			tempB_1 = checkBit(R[x], 31) != checkBit(R[y], 31);
			tempB_2 = checkBit(R[z], 31) != checkBit(R[x], 31);

			if (tempB_1 && tempB_2) {
				setOV(R[31], true);
			} else {
				setOV(R[31], false);
			}

			if (checkBit(temp64_1, 32)) {
				setCY(R[31], true);
			} else {
				setCY(R[31], false);
			}

//			0x????????:	sub rz,pc,ry             	Rz=PC-Ry=0x????????,SR=0x????????
			sprintf(instrucao, "sub %s,%s,%s", specifyRegL(z).c_str(),
					specifyRegL(x).c_str(), specifyRegL(y).c_str());
			// Formatacao de saida em tela (deve mudar para o arquivo de saida)
			fprintf(write, "0x%08X:\t%-25s\t%s=%s-%s=0x%08X,SR=0x%08X\n", pc,
					instrucao, specifyRegU(z).c_str(), specifyRegU(x).c_str(),
					specifyRegU(y).c_str(), temp64_1, R[31]);

			break;

			// l8
		case 0b011000:

			pc = R[29];
			z = (R[28] & (0b11111 << 21)) >> 21;
			x = (R[28] & (0b11111 << 16)) >> 16;
			i16bits = R[28] & 0xFFFF;

			temp32_1 = (R[x] + signExtension32(i16bits));
			temp32_2 = temp32_1 % 4;

			temp32_3 = MEM32[(R[x] + signExtension32(i16bits)) >> 2];
//			temp32_2 = temp32_1 % 4;

			if (temp32_2 == 0) {
				R[z] = (temp32_3 >> 24) & 0xFF;
			} else if (temp32_2 == 1) {
				R[z] = (temp32_3 >> 16) & 0xFF;
			} else if (temp32_2 == 2) {
				R[z] = (temp32_3 >> 8) & 0xFF;
			} else {
				R[z] = temp32_3 & 0xFF;
			}

//			R[z] = MEM32[(R[x] + signExtension32(i16bits)) >> 2];

// 0x????????:	l8 rz,[rx+-s]            	Rz=MEM[0x????????]=0x??
			sprintf(instrucao, "l8 %s,[%s%s%i]", specifyRegL(z).c_str(),
					specifyRegL(x).c_str(), (i16bits >= 0) ? ("+") : (""),
					i16bits);
			fprintf(write, "0x%08X:\t%-25s\t%s=MEM[0x%08X]=0x%02X\n", pc,
					instrucao, specifyRegU(z).c_str(),
					R[x] + signExtension32(i16bits), R[z]);
			break;

			// l16
		case 0b011001:

			pc = R[29];
			z = (R[28] & (0b11111 << 21)) >> 21;
			x = (R[28] & (0b11111 << 16)) >> 16;
			i16bits = R[28] & 0xFFFF;

			temp32_1 = ((R[x] + signExtension32(i16bits)) << 1);
			temp32_2 = temp32_1 % 4;

			temp32_3 = MEM32[((R[x] + signExtension32(i16bits)) << 1) >> 2];
			//			temp32_2 = temp32_1 % 4;

			if (temp32_2 < 2) {
				R[z] = (temp32_3 >> 16) & 0xFFFF;
			} else {
				R[z] = temp32_3 & 0xFFFF;
			}

			// 0x????????:	l16 rz,[rx+-s]           	Rz=MEM[0x????????]=0x????
			sprintf(instrucao, "l16 %s,[%s%s%i]", specifyRegL(z).c_str(),
					specifyRegL(x).c_str(), (i16bits >= 0) ? ("+") : (""),
					i16bits);
			fprintf(write, "0x%08X:\t%-25s\t%s=MEM[0x%08X]=0x%04X\n", pc,
					instrucao, specifyRegU(z).c_str(),
					(R[x] + signExtension32(i16bits)) << 1, R[z]);

			break;
			// l32
		case 0b011010:

			pc = R[29];
			z = (R[28] & (0b11111 << 21)) >> 21;
			x = (R[28] & (0b11111 << 16)) >> 16;
			i16bits = R[28] & 0xFFFF;

			R[z] = MEM32[((R[x] + signExtension32(i16bits)) << 2) >> 2];

			// 0x????????:	l32 rz,[rx+-s]           	Rz=MEM[0x????????]=0x????????
			sprintf(instrucao, "l32 %s,[%s%s%i]", specifyRegL(z).c_str(),
					specifyRegL(x).c_str(), (i16bits >= 0) ? ("+") : (""),
					i16bits);
			fprintf(write, "0x%08X:\t%-25s\t%s=MEM[0x%08X]=0x%08X\n", pc,
					instrucao, specifyRegU(z).c_str(),
					(R[x] + signExtension32(i16bits)) << 2, R[z]);

			break;

			// s8
		case 0b011011:

			pc = R[29];
			z = (R[28] & (0b11111 << 21)) >> 21;
			x = (R[28] & (0b11111 << 16)) >> 16;
			i16bits = R[28] & 0xFFFF;

			temp32_1 = (R[x] + signExtension32(i16bits));
			temp32_2 = temp32_1 % 4;

			temp32_3 = MEM32[(R[x] + signExtension32(i16bits)) >> 2];

			if (temp32_2 == 0) {
				R[z] = ((temp32_3 >> 24) & 0xFF) << 24;
				temp32_1 = (temp32_3 >> 24) & 0xFF;
			} else if (temp32_2 == 1) {
				R[z] = ((temp32_3 >> 16) & 0xFF) << 16;
				temp32_1 = (temp32_3 >> 16) & 0xFF;
			} else if (temp32_2 == 2) {
				R[z] = ((temp32_3 >> 8) & 0xFF) << 8;
				temp32_1 = (temp32_3 >> 8) & 0xFF;
			} else {
				R[z] = temp32_3 & 0xFF;
				temp32_1 = temp32_3 & 0xFF;
			}

			MEM32[(R[x] + signExtension32(i16bits)) >> 2] = R[z]
					| MEM32[(R[x] + signExtension32(i16bits)) >> 2];

			// 0x????????:	s8 [rx+-s],rz            	MEM[0x????????]=Rz=0x??
			sprintf(instrucao, "s8 [%s%s%i],%s", specifyRegL(x).c_str(),
					(i16bits >= 0) ? ("+") : (""), i16bits,
					specifyRegL(z).c_str());
			fprintf(write, "0x%08X:\t%-25s\tMEM[0x%08X]=%s=0x%02X\n", pc,
					instrucao, R[x] + signExtension32(i16bits),
					specifyRegU(z).c_str(), temp32_1);
			break;

			// s16
		case 0b011100:
			pc = R[29];
			z = (R[28] & (0b11111 << 21)) >> 21;
			x = (R[28] & (0b11111 << 16)) >> 16;
			i16bits = R[28] & 0xFFFF;

			temp32_1 = ((R[x] + signExtension32(i16bits)) << 1);
			temp32_2 = temp32_1 % 4;

			temp32_3 = MEM32[((R[x] + signExtension32(i16bits)) << 1) >> 2];

			if (temp32_2 < 2) {
				R[z] = ((temp32_3 >> 16) & 0xFFFF) << 16;
				temp32_1 = (temp32_3 >> 16) & 0xFFFF;
			} else {
				R[z] = temp32_3 & 0xFFFF;
				temp32_1 = temp32_3 & 0xFFFF;
			}

			MEM32[((R[x] + signExtension32(i16bits)) << 1) >> 2] = R[z]
					| MEM32[((R[x] + signExtension32(i16bits)) << 1) >> 2];

			// 0x????????:	s16 [rx+-s],rz           	MEM[0x????????]=Rz=0x????
			sprintf(instrucao, "s16 [%s%s%i],%s", specifyRegL(x).c_str(),
					(i16bits >= 0) ? ("+") : (""), i16bits,
					specifyRegL(z).c_str());
			fprintf(write, "0x%08X:\t%-25s\tMEM[0x%08X]=%s=0x%04X\n", pc,
					instrucao, (R[x] + signExtension32(i16bits)) << 1,
					specifyRegU(z).c_str(), temp32_1);
			break;

			// s32
		case 0b011101:
			pc = R[29];
			z = (R[28] & (0b11111 << 21)) >> 21;
			x = (R[28] & (0b11111 << 16)) >> 16;
			i16bits = R[28] & 0xFFFF;

			R[z] = MEM32[((R[x] + signExtension32(i16bits)) << 2) >> 2];

			MEM32[((R[x] + signExtension32(i16bits)) << 2) >> 2] = R[z];

			// 0x????????:	s32 [rx+-s],rz           	MEM[0x????????]=Rz=0x????????
			sprintf(instrucao, "s32 [%s%s%i],%s", specifyRegL(x).c_str(),
					(i16bits >= 0) ? ("+") : (""), i16bits,
					specifyRegL(z).c_str());
			fprintf(write, "0x%08X:\t%-25s\tMEM[0x%08X]=%s=0x%08X\n", pc,
					instrucao, (R[x] + signExtension32(i16bits)) << 2,
					specifyRegU(z).c_str(), R[z]);
			break;

			// bun
		case 0b110111:

			// Saving PC
			pc = R[29];

			// R[29] = R[29] + ((R[28] & 0x3FFFFFF) << 2) // Add the + 4 later
			R[29] = R[29] + ((R[28] & 0x3FFFFFF) << 2);
			// Formatacao da instrucao
			sprintf(instrucao, "bun %i", R[28] & 0x3FFFFFF);
			// Formatacao de saida em tela (deve mudar para o arquivo de saida)
			fprintf(write, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao,
					R[29] + 4);
			break;

			// int
		case 0b111111:
			// Parar a execucao
			execution = false;
			// 0x????????:	int u                    	CR=0x????????,PC=0x????????
			sprintf(instrucao, "int 0");
			// Formatacao de saida em tela (deve mudar para o arquivo de saida)
			fprintf(write, "0x%08X:\t%-25s\tCR=0x00000000,PC=0x00000000\n",
					R[29], instrucao);
			break;

			// cmpi
		case 0b010111:

			pc = R[29];
			x = (R[28] & (0b11111 << 16)) >> 16;
			i16bits = R[28] & 0xFFFF;

			temp32_1 = extend16To32(i16bits);

			temp64_1 = extend32To64(R[x]) - extend32To64(temp32_1);

			temp32_3 = static_cast<uint32_t>(temp64_1);

			if (temp64_1 == 0) {
				setZN(R[31], true);
			} else {
				setZN(R[31], false);
			}

			if (checkBit(temp32_3, 31)) {
				setSN(R[31], true);
			} else {
				setSN(R[31], false);
			}

			tempB_1 = checkBit(R[x], 31) != checkBit(i16bits, 15);
			tempB_2 = checkBit(temp32_3, 31) != checkBit(R[x], 31);

			if (tempB_1 && tempB_2) {
				setOV(R[31], true);
			} else {
				setOV(R[31], false);
			}

			if (checkBit(temp64_1, 32)) {
				setCY(R[31], true);
			} else {
				setCY(R[31], false);
			}

			// 0x????????:	cmpi rx,s                	SR=0x????????
			sprintf(instrucao, "cmpi %s,%i", specifyRegL(x).c_str(),
					extend16To32(i16bits));
			fprintf(write, "0x%08X:\t%-25s\tSR=0x%08X\n", pc, instrucao, R[31]);
			break;

			// cmp
		case 0b000101:

			pc = R[29];
			x = (R[28] & (0b11111 << 16)) >> 16;
			y = (R[28] & (0b11111 << 11)) >> 11;

			temp64_1 = static_cast<uint64_t>(R[x])
					- static_cast<uint64_t>(R[y]);

			if (temp64_1 == 0) {
				setZN(R[31], true);
			} else {
				setZN(R[31], false);
			}

			if (checkBit(temp64_1, 31)) {
				setSN(R[31], true);
			} else {
				setSN(R[31], false);
			}

			tempB_1 = checkBit(R[x], 31) != checkBit(R[y], 31);
			tempB_2 = checkBit(temp64_1, 31) != checkBit(R[x], 31);

			if (tempB_1 && tempB_2) {
				setOV(R[31], true);
			} else {
				setOV(R[31], false);
			}

			if (checkBit(temp64_1, 32)) {
				setCY(R[31], true);
			} else {
				setCY(R[31], false);
			}

//			0x????????:	cmp rx,sr                	SR=0x????????
			sprintf(instrucao, "cmp %s,%s", specifyRegL(x).c_str(),
					specifyRegL(y).c_str());
			fprintf(write, "0x%08X:\t%-25s\tSR=0x%08X\n", pc, instrucao, R[31]);
			break;

			// subi
		case 0b010011:

			pc = R[29];

			z = (R[28] & (0b11111 << 21)) >> 21;
			x = (R[28] & (0b11111 << 16)) >> 16;
			i16bits = R[28] & 0xFFFF;

			Rz_subi = static_cast<uint64_t>(R[x])
					- static_cast<uint64_t>(extend26To32(i16bits));
			R[z] = Rz_subi;

			if (Rz_subi == 0) {
				// setting to 1
				setZN(R[31], true);
			} else {
				// setting to 0
				setZN(R[31], false);
			}

			if (checkBit(Rz_subi, 31)) {
				// setting to 1 true positive
				setSN(R[31], true);
			} else {
				setSN(R[31], false);
			}

			Rx_31_subi = checkBit(R[x], 31);
			i_15_subi = checkBit(i16bits, 15);
			Rz_31_subi = checkBit(Rz_subi, 31);

			if ((Rx_31_subi != i_15_subi) && (Rz_31_subi != Rx_31_subi)) {
				setOV(R[31], true);
			} else {
				setOV(R[31], false);
			}

			if (checkBit(rz, 32)) {
				setCY(R[31], true);
			} else {
				setCY(R[31], false);
			}

			// 0x????????:	subi rz,rx,s             	Rz=Rx-0x????????=0x????????,SR=0x????????
			sprintf(instrucao, "subi %s,%s,%i", specifyRegL(z).c_str(),
					specifyRegL(x).c_str(), signExtension32(i16bits));
			fprintf(write, "0x%08X:\t%-25s\t%s=%s-0x%08X=0x%08X,SR=0x%08X\n",
					pc, instrucao, specifyRegU(z).c_str(),
					specifyRegU(x).c_str(), extend26To32(i16bits), R[z], R[31]);
			break;

			// mul
		case 0b000100:

			multOperand = (R[28] & (0b111 << 8)) >> 8;

			switch (multOperand) {

			// mul
			case 0b000000:

				pc = R[29];
				l = R[28] & static_cast<uint32_t>(0b11111);
				x = (R[28] & (0b11111 << 16)) >> 16;
				y = (R[28] & (0b11111 << 11)) >> 11;
				z = (R[28] & (0b11111 << 21)) >> 21;

				// R[z] : R[x] = (R[z]:R[y]) / 2^{l+1}

				temp64_1 = static_cast<uint64_t>(R[x])
						* static_cast<uint64_t>(R[y]);

				verifyZero(x, 1, temp64_1);
				verifyZero(y, 2, temp64_1);

				// Extracting the 32 most significant bits
				R[l] = setRegister(l,
						static_cast<uint32_t>(temp64_1 >> 32) & 0xFFFFFFFF);

				// Extracting the least significant bits
				R[z] = setRegister(z,
						static_cast<uint32_t>(temp64_1) & 0xFFFFFFFF);

				if (temp64_1 == 0) {
					setZN(R[31], true);
				} else {
					setZN(R[31], false);
				}

				if (R[l] != 0) {
					setCY(R[31], true);
				} else {
					setCY(R[31], false);
				}

				//	0x????????:	mul rl,rz,rx,ry          	Rl:Rz=Rx*Ry=0x????????????????,SR=0x????????
				sprintf(instrucao, "mul %s,%s,%s,%s", specifyRegL(l).c_str(),
						specifyRegL(z).c_str(), specifyRegL(x).c_str(),
						specifyRegL(y).c_str());
				fprintf(write,
						"0x%08X:\t%-25s\t%s:%s=%s*%s=0x%016llX,SR=0x%08X\n", pc,
						instrucao, specifyRegU(l).c_str(),
						specifyRegU(z).c_str(), specifyRegU(x).c_str(),
						specifyRegU(y).c_str(), temp64_1, R[31]);
				break;

				// sll
			case 0b001:

				pc = R[29];
				l = R[28] & static_cast<uint32_t>(0b11111);
				x = (R[28] & (0b11111 << 16)) >> 16;
				y = (R[28] & (0b11111 << 11)) >> 11;
				z = (R[28] & (0b11111 << 21)) >> 21;

				// R[z] : R[x] = (R[z]:R[y]) * 2^{l+1}

				temp64_1 = (static_cast<uint64_t>(R[z]) << 32);
				temp64_1 = temp64_1 | static_cast<uint64_t>(R[y]);

				temp64_2 = static_cast<uint64_t>(std::pow(2, l + 1));
				temp64_1 = temp64_1 * temp64_2;

				verifyZero(z, 1, temp64_1);
				verifyZero(y, 2, temp64_1);

				// Extracting the 32 most significant bits
				R[z] = setRegister(z,
						static_cast<uint32_t>(temp64_1 >> 32) & 0xFFFFFFFF);

				// Extracting the least significant bits
				R[x] = setRegister(x,
						static_cast<uint32_t>(temp64_1) & 0xFFFFFFFF);

				if (temp64_1 == 0) {
					setZN(R[31], true);
				} else {
					setZN(R[31], false);
				}

				if (R[z] != 0) {
					setCY(R[31], true);
				} else {
					setCY(R[31], false);
				}

				//	0x????????:	sll rz,rx,ry,u           	Rz:Rx=Rz:Ry<<u=0x????????????????,SR=0x????????
				sprintf(instrucao, "sll %s,%s,%s,%u", specifyRegL(z).c_str(),
						specifyRegL(x).c_str(), specifyRegL(y).c_str(), l);
				fprintf(write,
						"0x%08X:\t%-25s\t%s:%s=%s:%s<<%u=0x%016llX,SR=0x%08X\n",
						pc, instrucao, specifyRegU(z).c_str(),
						specifyRegU(x).c_str(), specifyRegU(z).c_str(),
						specifyRegU(y).c_str(), l + 1, temp64_1, R[31]);
				break;

				// muls
			case 0b010:

				pc = R[29];
				l = R[28] & static_cast<uint32_t>(0b11111);
				x = (R[28] & (0b11111 << 16)) >> 16;
				y = (R[28] & (0b11111 << 11)) >> 11;
				z = (R[28] & (0b11111 << 21)) >> 21;

				// R[l] : R[z] = R[x] * R[y]

				tmp64Mul_1 = toTwoComplement64bits(R[x]);
				tmp64Mul_2 = toTwoComplement64bits(R[y]);

				temp64_1 = tmp64Mul_1 * tmp64Mul_2;

				verifyZero(x, 1, temp64_1);
				verifyZero(y, 2, temp64_1);

				// Extracting the 32 most significant bits
				R[l] = setRegister(l,
						static_cast<uint32_t>(temp64_1 >> 32) & 0xFFFFFFFF);

				// Extracting the least significant bits
				R[z] = setRegister(z,
						static_cast<uint32_t>(temp64_1) & 0xFFFFFFFF);

				if (temp64_1 == 0) {
					setZN(R[31], true);
				} else {
					setZN(R[31], false);
				}

				if (R[l] != 0) {
					setOV(R[31], true);
				} else {
					setOV(R[31], false);
				}

				//	0x????????:	muls rl,rz,rx,ry         	Rl:Rz=Rx*Ry=0x????????????????,SR=0x????????
				sprintf(instrucao, "muls %s,%s,%s,%s", specifyRegL(l).c_str(),
						specifyRegL(z).c_str(), specifyRegL(x).c_str(),
						specifyRegL(y).c_str());
				fprintf(write,
						"0x%08X:\t%-25s\t%s:%s=%s*%s=0x%016llX,SR=0x%08X\n", pc,
						instrucao, specifyRegU(l).c_str(),
						specifyRegU(z).c_str(), specifyRegU(x).c_str(),
						specifyRegU(y).c_str(), temp64_1, R[31]);
				break;
				// sla
			case 0b011:

				pc = R[29];
				l = R[28] & static_cast<uint32_t>(0b11111);
				x = (R[28] & (0b11111 << 16)) >> 16;
				y = (R[28] & (0b11111 << 11)) >> 11;
				z = (R[28] & (0b11111 << 21)) >> 21;

				// R[z] : R[x] = (R[z]:R[y]) * 2^{l+1}

				temp64_1 = (static_cast<uint64_t>(R[z]) << 32);
				temp64_1 = temp64_1 | static_cast<uint64_t>(R[y]);

				temp64_2 = static_cast<uint64_t>(std::pow(2, l + 1));
				temp64_1 = temp64_1 * temp64_2;

				verifyZero(z, 1, temp64_1);
				verifyZero(y, 2, temp64_1);

				// Extracting the 32 most significant bits
				R[z] = setRegister(z,
						static_cast<uint32_t>(temp64_1 >> 32) & 0xFFFFFFFF);

				// Extracting the least significant bits
				R[x] = setRegister(x,
						static_cast<uint32_t>(temp64_1) & 0xFFFFFFFF);

				if (temp64_1 == 0) {
					setZN(R[31], true);
				} else {
					setZN(R[31], false);
				}

				if (R[z] != 0) {
					setOV(R[31], true);
				} else {
					setOV(R[31], false);
				}

//				0x????????:	sla rz,rx,ry,u           	Rz:Rx=Rz:Ry<<u=0x????????????????,SR=0x????????
				sprintf(instrucao, "sla %s,%s,%s,%u", specifyRegL(z).c_str(),
						specifyRegL(x).c_str(), specifyRegL(y).c_str(), l);
				fprintf(write,
						"0x%08X:\t%-25s\t%s:%s=%s:%s<<%u=0x%016X,SR=0x%08X\n",
						pc, instrucao, specifyRegU(z).c_str(),
						specifyRegU(x).c_str(), specifyRegU(z).c_str(),
						specifyRegU(y).c_str(), l + 1, temp64_1, R[31]);

				break;

				// div
			case 0b100:

				pc = R[29];
				l = R[28] & static_cast<uint32_t>(0b11111);
				x = (R[28] & (0b11111 << 16)) >> 16;
				y = (R[28] & (0b11111 << 11)) >> 11;
				z = (R[28] & (0b11111 << 21)) >> 21;

				if (y == 0) {

					setZD(R[31], true);

				} else {
					R[l] = R[x] % R[y];
					R[z] = R[x] / R[y];

					if (R[z] == 0) {
						setZN(R[31], true);
					} else {
						setZN(R[31], false);
					}

					if (R[y] == 0) {
						setZD(R[31], true);
					} else {
						setZD(R[31], false);
					}

					if (R[l] != 0) {
						setCY(R[31], true);
					} else {
						setCY(R[31], false);
					}

				}

				//				0x????????:	div rl,rz,rx,ry          	Rl=Rx%Ry=0x????????,Rz=Rx/Ry=0x????????,SR=0x????????
				sprintf(instrucao, "div %s,%s,%s,%s", specifyRegL(l).c_str(),
						specifyRegL(z).c_str(), specifyRegL(x).c_str(),
						specifyRegL(y).c_str());
				fprintf(write,
						"0x%08X:\t%-25s\t%s=%s%%%s=0x%08X,%s=%s/%s=0x%08X,SR=0x%08X\n",
						pc, instrucao, specifyRegU(l).c_str(),
						specifyRegU(x).c_str(), specifyRegU(y).c_str(), R[l],
						specifyRegU(z).c_str(), specifyRegU(x).c_str(),
						specifyRegU(y).c_str(), R[z], R[31]);

				break;

				// srl
			case 0b101:

				pc = R[29];
				l = R[28] & static_cast<uint32_t>(0b11111);
				x = (R[28] & (0b11111 << 16)) >> 16;
				y = (R[28] & (0b11111 << 11)) >> 11;
				z = (R[28] & (0b11111 << 21)) >> 21;

				temp64_1 = (static_cast<uint64_t>(R[z]) << 32);
				temp64_1 = temp64_1 | static_cast<uint64_t>(R[y]);
				temp64_1 = temp64_1 / std::pow(2, l + 1);

				// Extracting the 32 most significant bits
				R[z] = static_cast<uint32_t>(temp64_1 >> 32) & 0xFFFFFFFF;

				// Extracting the least significant bits
				R[x] = static_cast<uint32_t>(temp64_1 & 0xFFFFFFFF);

				if (temp64_1 == 0) {
					setZN(R[31], true);
				} else {
					setZN(R[31], false);
				}

				if (R[z] != 0) {
					setOV(R[31], true);
				} else {
					setOV(R[31], false);
				}

//				0x????????:	srl rz,rx,ry,u           	Rz:Rx=Rz:Ry>>u=0x????????????????,SR=0x????????
				sprintf(instrucao, "srl %s,%s,%s,%u", specifyRegL(z).c_str(),
						specifyRegL(x).c_str(), specifyRegL(y).c_str(),
						R[28] & 0x3FFFFFF);
				fprintf(write,
						"0x%08X:\t%-25s\t%s:%s=%s*%s>>%u=0x%016" PRIu64 ",SR=0x%08X\n",
						pc, instrucao, specifyRegU(z).c_str(),
						specifyRegU(x).c_str(), specifyRegU(z).c_str(),
						specifyRegU(y).c_str(), R[28] & 0x3FFFFFF, temp64_1,
						R[31]);

				break;

				// divs
			case 0b110:

				pc = R[29];
				l = R[28] & static_cast<uint32_t>(0b11111);
				x = (R[28] & (0b11111 << 16)) >> 16;
				y = (R[28] & (0b11111 << 11)) >> 11;
				z = (R[28] & (0b11111 << 21)) >> 21;

				tempB_1 = signResultMod(R[x], R[y]);

				temp32_1 = toTwoComplement32bitsIfNegative(R[x]);
				temp32_2 = toTwoComplement32bitsIfNegative(R[y]);

				temp32_3 = temp32_1 % temp32_2;
				temp32_3 = toTwoComplement32bitsNegative(temp32_3, tempB_1);

				R[l] = temp32_3;

				tempB_1 = signResult(R[x], R[y]);
				temp32_3 = temp32_1 / temp32_2;
				temp32_3 = toTwoComplement32bitsNegative(temp32_3, tempB_1);

				R[z] = temp32_3;

				if (R[z] == 0) {
					setZN(R[31], true);
				} else {
					setZN(R[31], false);
				}

				if (R[y] == 0) {
					setZD(R[31], true);
				} else {
					setZD(R[31], false);
				}

				if (R[l] != 0) {
					setOV(R[31], true);
				} else {
					setOV(R[31], false);
				}

				//	0x????????:	divs rl,rz,rx,ry         	Rl=Rx%Ry=0x????????,Rz=Rx/Ry=0x????????,SR=0x????????
				sprintf(instrucao, "divs %s,%s,%s,%s", specifyRegL(l).c_str(),
						specifyRegL(z).c_str(), specifyRegL(x).c_str(),
						specifyRegL(y).c_str());
				fprintf(write,
						"0x%08X:\t%-25s\t%s=%s%%%s=0x%08X,%s=%s/%s=0x%08X,SR=0x%08X\n",
						pc, instrucao, specifyRegU(l).c_str(),
						specifyRegU(x).c_str(), specifyRegU(y).c_str(), R[l],
						specifyRegU(z).c_str(), specifyRegU(x).c_str(),
						specifyRegU(y).c_str(), R[z], R[31]);

				break;

				// sra
			case 0b111:

				pc = R[29];
				l = R[28] & static_cast<uint32_t>(0b11111);
				x = (R[28] & (0b11111 << 16)) >> 16;
				y = (R[28] & (0b11111 << 11)) >> 11;
				z = (R[28] & (0b11111 << 21)) >> 21;

//				Aritmético (com sinal)
//				▶ R[z] : R[x] = (R[z] : R[y]) ÷ 2l4:0+1

				temp64_1 = (static_cast<uint64_t>(R[z]) << 32);
				temp64_1 = temp64_1 | static_cast<uint64_t>(R[y]);

				tempB_1 = checkBit(temp64_1, 63);

				temp64_1 = temp64_1 >> (l + 1);

				temp64_1 = extend64To64(temp64_1, tempB_1);

				verifyZero(z, 1, temp64_1);
				verifyZero(y, 2, temp64_1);

				// Extracting the 32 most significant bits
				R[z] = setRegister(z,
						static_cast<uint32_t>(temp64_1 >> 32) & 0xFFFFFFFF);

				// Extracting the least significant bits
				R[x] = setRegister(x,
						static_cast<uint32_t>(temp64_1) & 0xFFFFFFFF);

				if (temp64_1 == 0) {
					setZN(R[31], true);
				} else {
					setZN(R[31], false);
				}

				if (R[z] != 0) {
					setOV(R[31], true);
				} else {
					setOV(R[31], false);
				}

				//	0x????????:	sra rz,rx,ry,u           	Rz:Ry=Rz:Rx>>u=0x????????????????,SR=0x????????
				sprintf(instrucao, "sra %s,%s,%s,%u", specifyRegL(z).c_str(),
						specifyRegL(x).c_str(), specifyRegL(y).c_str(), l);
				fprintf(write,
						"0x%08X:\t%-25s\t%s:%s=%s:%s>>%u=0x%016llX,SR=0x%08X\n",
						pc, instrucao, specifyRegU(z).c_str(),
						specifyRegU(y).c_str(), specifyRegU(z).c_str(),
						specifyRegU(x).c_str(), l + 1, temp64_1, R[31]);
				break;
			}

			break;

			// bne
		case 0b110100:

			pc = R[29];
			i32bits = R[28] & 0x03FFFFFF;

			// If ZN != from 1 then execute it
			if (!getZN(R[31])) {

				R[29] = R[29] + (extend26To32(i32bits) << 2);

				// 0x????????:	bne s                    	PC=0x????????
				sprintf(instrucao, "bne %i", extend26To32(i32bits));
				fprintf(write, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao,
						R[29] + 4);
			} else {

				// 0x????????:	bne s                    	PC=0x????????
				sprintf(instrucao, "bne %i", extend26To32(i32bits));
				fprintf(write, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao,
						R[29] + 4);
			}
			break;

			// bae
		case 0b101010:

			pc = R[29];
			i32bits = R[28] & 0x03FFFFFF;

			//			0x????????:	bae s                    	PC=0x????????
			if (!getCY(R[31])) {

				R[29] = R[29] + (extend26To32(i32bits) << 2);

				sprintf(instrucao, "bae %i", extend26To32(i32bits));
				fprintf(write, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao,
						R[29] + 4);

			} else {
				sprintf(instrucao, "bae %i", extend26To32(i32bits));
				fprintf(write, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao,
						R[29] + 4);
			}

			break;

			// bat
		case 0b101011:

			pc = R[29];
			i32bits = R[28] & 0x03FFFFFF;

			//			0x????????:	bat s                    	PC=0x????????
			if (!getZN(R[31]) && !getCY(R[31])) {

				R[29] = R[29] + (extend26To32(i32bits) << 2);

				sprintf(instrucao, "bat %i", extend26To32(i32bits));
				fprintf(write, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao,
						R[29] + 4);

			} else {
				sprintf(instrucao, "bat %i", extend26To32(i32bits));
				fprintf(write, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao,
						R[29] + 4);
			}

			break;

			// bbe
		case 0b101100:

			pc = R[29];
			i32bits = R[28] & 0x03FFFFFF;

			//	0x????????:	bbe s                    	PC=0x????????
			if (getZN(R[31]) || getCY(R[31])) {

				R[29] = R[29] + (extend26To32(i32bits) << 2);

				sprintf(instrucao, "bbe %i", extend26To32(i32bits));
				fprintf(write, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao,
						R[29] + 4);

			} else {
				sprintf(instrucao, "bbe %i", extend26To32(i32bits));
				fprintf(write, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao,
						R[29] + 4);
			}

			break;

			// bbt
		case 0b101101:

			pc = R[29];
			i32bits = R[28] & 0x03FFFFFF;

			//	0x????????:	bbt s                    	PC=0x????????
			if (getCY(R[31])) {

				R[29] = R[29] + (extend26To32(i32bits) << 2);

				sprintf(instrucao, "bbt %i", extend26To32(i32bits));
				fprintf(write, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao,
						R[29] + 4);

			} else {
				sprintf(instrucao, "bbt %i", extend26To32(i32bits));
				fprintf(write, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao,
						R[29] + 4);
			}

			break;

			// beq
		case 0b101110:

			pc = R[29];
			i32bits = R[28] & 0x03FFFFFF;

			//	0x????????:	beq s                    	PC=0x????????
			if (getZN(R[31])) {

				R[29] = R[29] + (extend26To32(i32bits) << 2);

				sprintf(instrucao, "beq %i", extend26To32(i32bits));
				fprintf(write, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao,
						R[29] + 4);

			} else {
				sprintf(instrucao, "beq %i", extend26To32(i32bits));
				fprintf(write, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao,
						R[29] + 4);
			}

			break;

			// bge
		case 0b101111:

			pc = R[29];
			i32bits = R[28] & 0x03FFFFFF;

			//	0x????????:	bge s                    	PC=0x????????
			if (getSN(R[31]) == getOV(R[31])) {

				R[29] = R[29] + (extend26To32(i32bits) << 2);

				sprintf(instrucao, "bge %i", extend26To32(i32bits));
				fprintf(write, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao,
						R[29] + 4);

			} else {
				sprintf(instrucao, "bge %i", extend26To32(i32bits));
				fprintf(write, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao,
						R[29] + 4);
			}

			break;

			// bgt
		case 0b110000:

			pc = R[29];
			i32bits = R[28] & 0x03FFFFFF;

			//	0x????????:	bgt s                    	PC=0x????????
			if (!getZN(R[31]) && !getSN(R[31])) {

				R[29] = R[29] + (extend26To32(i32bits) << 2);

				sprintf(instrucao, "bgt %i", extend26To32(i32bits));
				fprintf(write, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao,
						R[29] + 4);

			} else {
				sprintf(instrucao, "bgt %i", extend26To32(i32bits));
				fprintf(write, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao,
						R[29] + 4);
			}

			break;

			// biv
		case 0b110001:

			pc = R[29];
			i32bits = R[28] & 0x03FFFFFF;

			if (i < 141) {
				setIV(R[31], true);

				R[29] = R[29] + (extend26To32(i32bits) << 2);

				//	0x????????:	biv s                    	PC=0x????????
				sprintf(instrucao, "biv %i", extend26To32(i32bits));
				fprintf(write, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao,
						R[29] + 4);
			} else {
				//	0x????????:	biv s                    	PC=0x????????
				sprintf(instrucao, "biv %i", extend26To32(i32bits));
				fprintf(write, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao,
						R[29] + 4);
			}

			break;

			// ble
		case 0b110010:

			pc = R[29];
			i32bits = R[28] & 0x03FFFFFF;

			//	0x????????:	ble s                    	PC=0x????????
			tempB_1 = getSN(R[31]) != getOV(R[31]);

			if (getZN(R[31]) || tempB_1) {

				R[29] = R[29] + (extend26To32(i32bits) << 2);

				sprintf(instrucao, "ble %i", extend26To32(i32bits));
				fprintf(write, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao,
						R[29] + 4);

			} else {
				sprintf(instrucao, "ble %i", extend26To32(i32bits));
				fprintf(write, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao,
						R[29] + 4);
			}

			break;

			// blt
		case 0b110011:

			pc = R[29];
			i32bits = R[28] & 0x03FFFFFF;

			//	0x????????:	blt s                    	PC=0x????????
			if (getSN(R[31]) != getOV(R[31])) {

				R[29] = R[29] + (extend26To32(i32bits) << 2);

				sprintf(instrucao, "blt %i", extend26To32(i32bits));
				fprintf(write, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao,
						R[29] + 4);

			} else {
				sprintf(instrucao, "blt %i", extend26To32(i32bits));
				fprintf(write, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao,
						R[29] + 4);
			}

			break;

			// bni
		case 0b110101:

			pc = R[29];
			i32bits = R[28] & 0x03FFFFFF;

			//	0x????????:	bni s                    	PC=0x????????
			if (!getIV(R[31])) {

				R[29] = R[29] + (extend26To32(i32bits) << 2);

				sprintf(instrucao, "bni %i", extend26To32(i32bits));
				fprintf(write, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao,
						R[29] + 4);

			} else {
				sprintf(instrucao, "bni %i", extend26To32(i32bits));
				fprintf(write, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao,
						R[29] + 4);
			}

			break;

			// bnz
		case 0b110110:

			pc = R[29];
			i32bits = R[28] & 0x03FFFFFF;

			//	0x????????:	bnz s                    	PC=0x????????
			if (!getZD(R[31])) {

				R[29] = R[29] + (extend26To32(i32bits) << 2);

				sprintf(instrucao, "bnz %i", extend26To32(i32bits));
				fprintf(write, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao,
						R[29] + 4);

			} else {
				sprintf(instrucao, "bnz %i", extend26To32(i32bits));
				fprintf(write, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao,
						R[29] + 4);
			}

			break;

			// bzd
		case 0b111000:

			pc = R[29];
			i32bits = R[28] & 0x03FFFFFF;

			if (getZD(R[31])) {
				//	0x????????:	bzd s                    	PC=0x????????
				R[29] = R[29] + (extend26To32(i32bits) << 2);

				sprintf(instrucao, "bzd %i", extend26To32(i32bits));
				fprintf(write, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao,
						R[29] + 4);
			} else {
				sprintf(instrucao, "bzd %i", extend26To32(i32bits));
				fprintf(write, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao,
						R[29] + 4);
			}

			break;

			// push
		case 0b001010:

			// saving Stack pointer
			pc = R[29];
			sp = R[30];

			z = (R[28] & (0b11111 << 21)) >> 21;
			x = (R[28] & (0b11111 << 16)) >> 16;
			y = (R[28] & (0b11111 << 11)) >> 11;
			v = (R[28] & (0b11111 << 6)) >> 6;            //[6-10]
			w = R[28] & (static_cast<uint32_t>(0b11111)); // [0,4]

			// SP = Stack pointer

			tmpCount = 0;

			if (v != 0) {
				MEM32[R[30] >> 2] = R[v];
				R[30] = R[30] - 4;

				tmpCount++;

				if (w != 0) {
					MEM32[R[30] >> 2] = R[w];
					R[30] = R[30] - 4;

					tmpCount++;

					if (x != 0) {
						MEM32[R[30] >> 2] = R[x];
						R[30] = R[30] - 4;

						tmpCount++;

						if (y != 0) {
							MEM32[R[30] >> 2] = R[y];
							R[30] = R[30] - 4;

							tmpCount++;

							if (z != 0) {
								MEM32[R[30] >> 2] = R[z];
								R[30] = R[30] - 4;

								tmpCount++;
							}
						}
					}
				}
			}

			// 0x????????:	push rv,rw,rx,ry,rz      	MEM[0x????????]{0x????????,0x????????,0x????????,0x????????,0x????????}={Rv,Rw,Rx,Ry,Rz}
			if (tmpCount == 1) {

				sprintf(instrucao, "push %s", specifyRegL(v).c_str());
				fprintf(write, "0x%08X:\t%-25s\tMEM[0x%08X]{0x%08X}={%s}\n", pc,
						instrucao, sp, R[v], specifyRegU(v).c_str());

			} else if (tmpCount == 2) {
				sprintf(instrucao, "push %s,%s", specifyRegL(v).c_str(),
						specifyRegL(w).c_str());
				fprintf(write,
						"0x%08X:\t%-25s\tMEM[0x%08X]{0x%08X,0x%08X}={%s,%s}\n",
						pc, instrucao, sp, R[v], R[w], specifyRegU(v).c_str(),
						specifyRegU(w).c_str());

			} else if (tmpCount == 3) {
				sprintf(instrucao, "push %s,%s,%s", specifyRegL(v).c_str(),
						specifyRegL(w).c_str(), specifyRegL(x).c_str());
				fprintf(write,
						"0x%08X:\t%-25s\tMEM[0x%08X]{0x%08X,0x%08X,0x%08X}={%s,%s,%s}\n",
						pc, instrucao, sp, R[v], R[w], R[x],
						specifyRegU(v).c_str(), specifyRegU(w).c_str(),
						specifyRegU(x).c_str());

			} else if (tmpCount == 4) {
				sprintf(instrucao, "push %s,%s,%s,%s", specifyRegL(v).c_str(),
						specifyRegL(w).c_str(), specifyRegL(x).c_str(),
						specifyRegL(y).c_str());
				fprintf(write,
						"0x%08X:\t%-25s\tMEM[0x%08X]{0x%08X,0x%08X,0x%08X,0x%08X}={%s,%s,%s,%s}\n",
						pc, instrucao, sp, R[v], R[w], R[x], R[y],
						specifyRegU(v).c_str(), specifyRegU(w).c_str(),
						specifyRegU(x).c_str(), specifyRegU(y).c_str());
			} else if (tmpCount == 5) {
				sprintf(instrucao, "push %s,%s,%s,%s,%s",
						specifyRegL(v).c_str(), specifyRegL(w).c_str(),
						specifyRegL(x).c_str(), specifyRegL(y).c_str(),
						specifyRegL(z).c_str());
				fprintf(write,
						"0x%08X:\t%-25s\tMEM[0x%08X]{0x%08X,0x%08X,0x%08X,0x%08X,0x%08X}={%s,%s,%s,%s,%s}\n",
						pc, instrucao, sp, R[v], R[w], R[x], R[y], R[z],
						specifyRegU(v).c_str(), specifyRegU(w).c_str(),
						specifyRegU(x).c_str(), specifyRegU(y).c_str(),
						specifyRegU(z).c_str());
			} else {
				sprintf(instrucao, "push -");
				fprintf(write, "0x%08X:\t%-25s\tMEM[0x%08X]{}={}\n", pc,
						instrucao, sp);
			}

			break;

			// muli
		case 0b010100:
			pc = R[29];
			x = (R[28] & (0b11111 << 16)) >> 16;
			z = (R[28] & (0b11111 << 21)) >> 21;
			i16bits = R[28] & 0xFFFF;

			temp32_1 = extend16To32(i16bits);

			tmp64Mul_1 = toTwoComplement64bits(R[x]);
			tmp64Mul_2 = toTwoComplement64bits(temp32_1);

			temp64_1 = tmp64Mul_1 * tmp64Mul_2;

			verifyZero(z, 2, temp64_1);

			// Extracting the 32 most significant bits
			temp32_2 = static_cast<uint32_t>(temp64_1 >> 32) & 0xFFFFFFFF;

			// Extracting the least significant bits
			R[z] = setRegister(z, static_cast<uint32_t>(temp64_1) & 0xFFFFFFFF);

			if (R[z] == 0) {
				setZN(R[31], true);
			} else {
				setZN(R[31], false);
			}

			if (temp32_2 != 0) {
				setOV(R[31], true);
			} else {
				setOV(R[31], false);
			}

			//	0x????????:	muli rz,rx,s             	Rz=Rx*0x????????=0x????????,SR=0x????????
			sprintf(instrucao, "muli %s,%s,%i", specifyRegL(z).c_str(),
					specifyRegL(x).c_str(), extend16To32(i16bits));
			fprintf(write, "0x%08X:\t%-25s\t%s=%s*0x%08X=0x%08X,SR=0x%08X\n",
					pc, instrucao, specifyRegU(z).c_str(),
					specifyRegU(x).c_str(), extend16To32(i16bits), R[z], R[31]);
			break;

			// divi
		case 0b010101:
			pc = R[29];
			x = (R[28] & (0b11111 << 16)) >> 16;
			z = (R[28] & (0b11111 << 21)) >> 21;
			i16bits = R[28] & 0xFFFF;

			if (i16bits == 0) {
				setZD(R[31], true);
			} else {
				temp32_2 = extend16To32(i16bits);

				tempB_1 = signResult(R[x], temp32_2);

				temp32_1 = toTwoComplement32bitsIfNegative(R[x]);
				temp32_2 = toTwoComplement32bitsIfNegative(temp32_2);

				temp32_3 = temp32_1 / temp32_2;

				temp32_3 = toTwoComplement32bitsNegative(temp32_3, tempB_1);

				// Extracting the least significant bits
				R[z] = setRegister(z,
						static_cast<uint32_t>(temp32_3) & 0xFFFFFFFF);

				if (R[z] == 0) {
					setZN(R[31], true);
				} else {
					setZN(R[31], false);
				}

				if (i16bits == 0) {
					setZD(R[31], true);
				} else {
					setZD(R[31], false);
				}

				setOV(R[31], false);
			}

			//	0x????????:	divi rz,rx,s             	Rz=Rx/0x????????=0x????????,SR=0x????????
			sprintf(instrucao, "divi %s,%s,%i", specifyRegL(z).c_str(),
					specifyRegL(x).c_str(), extend16To32(i16bits));
			fprintf(write, "0x%08X:\t%-25s\t%s=%s/0x%08X=0x%08X,SR=0x%08X\n",
					pc, instrucao, specifyRegU(z).c_str(),
					specifyRegU(x).c_str(), extend16To32(i16bits), R[z], R[31]);
			break;

			// modi
		case 0b010110:
			pc = R[29];
			x = (R[28] & (0b11111 << 16)) >> 16;
			z = (R[28] & (0b11111 << 21)) >> 21;
			i16bits = R[28] & 0xFFFF;

			temp32_1 = extend16To32(i16bits);
			tempB_1 = signResultMod(R[x], temp32_1);

			temp32_2 = toTwoComplement32bitsIfNegative(R[x]);
			temp32_3 = toTwoComplement32bitsIfNegative(temp32_1);

			temp32_3 = temp32_2 % temp32_3;
			temp32_3 = toTwoComplement32bitsNegative(temp32_3, tempB_1);

			R[z] = temp32_3;

			if (R[z] == 0) {
				setZN(R[31], true);
			} else {
				setZN(R[31], false);
			}

			if (i16bits == 0) {
				setZD(R[31], true);
			} else {
				setZD(R[31], false);
			}

			setOV(R[31], false);

			// 0x????????:	modi rz,rx,s             	Rz=Rx%0x????????=0x????????,SR=0x????????
			sprintf(instrucao, "modi %s,%s,%i", specifyRegL(z).c_str(),
					specifyRegL(x).c_str(), extend16To32(i16bits));
			fprintf(write, "0x%08X:\t%-25s\t%s=%s%%0x%08X=0x%08X,SR=0x%08X\n",
					pc, instrucao, specifyRegU(z).c_str(),
					specifyRegU(x).c_str(), extend16To32(i16bits), R[z], R[31]);

			break;

			// pop
		case 0b001011:

			// saving Stack pointer
			pc = R[29];
			sp = R[30];

			v = (R[28] & (0b11111 << 6)) >> 6;            //[6-10]
			w = R[28] & (static_cast<uint32_t>(0b11111)); // [0,4]
			x = (R[28] & (0b11111 << 16)) >> 16;
			z = (R[28] & (0b11111 << 21)) >> 21;
			y = (R[28] & (0b11111 << 11)) >> 11;

			tmpCount = 0;

			if (v != 0) {
				R[30] = R[30] + 4;
				R[v] = MEM32[R[30] >> 2];

				tmpCount++;

				if (w != 0) {
					R[30] = R[30] + 4;
					R[w] = MEM32[R[30] >> 2];

					tmpCount++;

					if (x != 0) {
						R[30] = R[30] + 4;
						R[x] = MEM32[R[30] >> 2];
						tmpCount++;
						if (y != 0) {
							R[30] = R[30] + 4;
							R[y] = MEM32[R[30] >> 2];
							tmpCount++;

							if (z != 0) {
								R[30] = R[30] + 4;
								R[z] = MEM32[R[30] >> 2];
								tmpCount++;
							}
						}
					}
				}
			}

			// 0x????????:	pop rv,rw,rx,ry,rz       	{Rv,Rw,Rx,Ry,Rz}=MEM[0x????????]{0x????????,0x????????,0x????????,0x????????,0x????????}
			if (tmpCount == 1) {

				sprintf(instrucao, "pop %s", specifyRegL(v).c_str());
				fprintf(write, "0x%08X:\t%-25s\t{%s}=MEM[0x%08X]{0x%08X}\n", pc,
						instrucao, specifyRegU(v).c_str(), sp, R[v]);

			} else if (tmpCount == 2) {

				sprintf(instrucao, "pop %s,%s", specifyRegL(v).c_str(),
						specifyRegL(w).c_str());
				fprintf(write,
						"0x%08X:\t%-25s\t{%s,%s}=MEM[0x%08X]{0x%08X,0x%08X}\n",
						pc, instrucao, specifyRegU(v).c_str(),
						specifyRegU(w).c_str(), sp, R[v], R[w]);

			} else if (tmpCount == 3) {

				sprintf(instrucao, "pop %s,%s,%s", specifyRegL(v).c_str(),
						specifyRegL(w).c_str(), specifyRegL(x).c_str());
				fprintf(write,
						"0x%08X:\t%-25s\t{%s,%s,%s}=MEM[0x%08X]{0x%08X,0x%08X,0x%08X}\n",
						pc, instrucao, specifyRegU(v).c_str(),
						specifyRegU(w).c_str(), specifyRegU(x).c_str(), sp,
						R[v], R[w], R[x]);

				// 0x????????:	pop rv,rw,rx,ry,rz       	{Rv,Rw,Rx,Ry,Rz}=MEM[0x????????]{0x????????,0x????????,0x????????,0x????????,0x????????}
			} else if (tmpCount == 4) {

				sprintf(instrucao, "pop %s,%s,%s,%s", specifyRegL(v).c_str(),
						specifyRegL(w).c_str(), specifyRegL(x).c_str(),
						specifyRegL(y).c_str());
				fprintf(write,
						"0x%08X:\t%-25s\t{%s,%s,%s,%s}=MEM[0x%08X]{0x%08X,0x%08X,0x%08X,0x%08X}\n",
						pc, instrucao, specifyRegU(v).c_str(),
						specifyRegU(w).c_str(), specifyRegU(x).c_str(),
						specifyRegU(y).c_str(), sp, R[v], R[w], R[x], R[y]);
			} else if (tmpCount == 5) {

				sprintf(instrucao, "pop %s,%s,%s,%s,%s", specifyRegL(v).c_str(),
						specifyRegL(w).c_str(), specifyRegL(x).c_str(),
						specifyRegL(y).c_str(), specifyRegL(z).c_str());
				fprintf(write,
						"0x%08X:\t%-25s\t{%s,%s,%s,%s,%s}=MEM[0x%08X]{0x%08X,0x%08X,0x%08X,0x%08X,0x%08X}\n",
						pc, instrucao, specifyRegU(v).c_str(),
						specifyRegU(w).c_str(), specifyRegU(x).c_str(),
						specifyRegU(y).c_str(), specifyRegU(z).c_str(), sp,
						R[v], R[w], R[x], R[y], R[z]);

			} else {
				sprintf(instrucao, "pop -");
				fprintf(write, "0x%08X:\t%-25s\t{}=MEM[0x%08X]{}\n", pc,
						instrucao, sp);
			}

			break;

			// call
		case 0b011110:

			sp = R[30];
			pc = R[29];

			MEM32[R[30] >> 2] = R[29] + 4;
			R[30] = R[30] - 4;

			x = (R[28] & (0b11111 << 16)) >> 16;
			i16bits = R[28] & 0xFFFF;

			R[29] = (R[x] + extend16To32(i16bits)) << 2;

			// 0x????????:	call [rx+-s]    PC=0x????????,MEM[0x????????]=0x????????
			sprintf(instrucao, "call [%s%s%i]", specifyRegL(x).c_str(),
					(extend16To32(i16bits) >= 0) ? ("+") : (""),
					extend16To32(i16bits));
			fprintf(write, "0x%08X:\t%-25s\tPC=0x%08X,MEM[0x%08X]=0x%08X\n", pc,
					instrucao, R[29], sp, MEM32[sp >> 2]);
			continue;
			break;

			// call 2
		case 0b111001:

			pc = R[29];
			sp = R[30];

			MEM32[R[30] >> 2] = R[29] + 4;
			R[30] = R[30] - 4;

			bitmask = (1 << 26) - 1;
			i32bits = R[28] & bitmask;

			R[29] = R[29] + (extend26To32(i32bits) << 2);

			// 0x????????:	call s PC=0x????????,MEM[0x????????]=0x????????
			sprintf(instrucao, "call %i", extend26To32(i32bits));
			fprintf(write, "0x%08X:\t%-25s\tPC=0x%08X,MEM[0x%08X]=0x%08X\n", pc,
					instrucao, R[29] + 4, sp, MEM32[sp >> 2]);

			break;

			// ret
		case 0b011111:
			pc = R[29];

			R[30] = R[30] + 4;
			R[29] = MEM32[R[30] >> 2];

			// 0x????????:	ret PC=MEM[0x????????]=0x????????
			sprintf(instrucao, "ret");
			// Formatacao de saida em tela (deve mudar para o arquivo de saida)
			fprintf(write, "0x%08X:\t%-25s\tPC=MEM[0x%08X]=0x%08X\n", pc,
					instrucao, R[30], R[29]);
			continue;
			break;

			// addi
		case 0b010010:

			pc = R[29];
			x = (R[28] & (0b11111 << 16)) >> 16;
			z = (R[28] & (0b11111 << 21)) >> 21;
			i16bits = R[28] & 0xFFFF;

			temp64_1 = static_cast<uint64_t>(R[x])
					+ static_cast<uint64_t>(extend16To32(i16bits));
			R[z] = temp64_1;

			if (R[z] == 0) {
				setZN(R[31], true);
			} else {
				setZN(R[31], false);
			}

			if (checkBit(temp64_1, 31)) {
				setSN(R[31], true);
			} else {
				setSN(R[31], false);
			}

			tempB_1 = checkBit(R[x], 31) == checkBit(i16bits, 15);
			tempB_2 = checkBit(R[z], 31) != checkBit(R[x], 31);

			if (tempB_1 && tempB_2) {
				setOV(R[31], true);
			} else {
				setOV(R[31], false);
			}

			if (checkBit(temp64_1, 32)) {
				setCY(R[31], true);
			} else {
				setCY(R[31], false);
			}

//			0x????????:	addi rz,rx,s             	Rz=Rx+0x????????=0x????????,SR=0x????????
			sprintf(instrucao, "addi %s,%s,%i", specifyRegL(z).c_str(),
					specifyRegL(x).c_str(), extend16To32(i16bits));
			// Formatacao de saida em tela (deve mudar para o arquivo de saida)
			fprintf(write, "0x%08X:\t%-25s\t%s=%s+0x%08X=0x%08X,SR=0x%08X\n",
					pc, instrucao, specifyRegU(z).c_str(),
					specifyRegU(x).c_str(), extend16To32(i16bits), temp64_1,
					R[31]);

			break;

			// and
		case 0b000110:
			pc = R[29];
			x = (R[28] & (0b11111 << 16)) >> 16;
			y = (R[28] & (0b11111 << 11)) >> 11;
			z = (R[28] & (0b11111 << 21)) >> 21;

			R[z] = R[x] & R[y];

			if (R[z] == 0) {
				setZN(R[31], true);
			} else {
				setZN(R[31], false);
			}

			if (checkBit(R[z], 31)) {
				setSN(R[31], true);
			} else {
				setSN(R[31], false);
			}

//			0x????????:	and rz,rx,ry             	Rz=Rx&Ry=0x????????,SR=0x????????
			sprintf(instrucao, "and %s,%s,%s", specifyRegL(z).c_str(),
					specifyRegL(x).c_str(), specifyRegL(y).c_str());
			// Formatacao de saida em tela (deve mudar para o arquivo de saida)
			fprintf(write, "0x%08X:\t%-25s\t%s=%s&%s=0x%08X,SR=0x%08X\n", pc,
					instrucao, specifyRegU(z).c_str(), specifyRegU(x).c_str(),
					specifyRegU(y).c_str(), R[z], R[31]);

			break;

			// or
		case 0b000111:

			pc = R[29];
			x = (R[28] & (0b11111 << 16)) >> 16;
			y = (R[28] & (0b11111 << 11)) >> 11;
			z = (R[28] & (0b11111 << 21)) >> 21;

			R[z] = R[x] | R[y];

			if (R[z] == 0) {
				setZN(R[31], true);
			} else {
				setZN(R[31], false);
			}

			if (checkBit(R[z], 31)) {
				setSN(R[31], true);
			} else {
				setSN(R[31], false);
			}

			//			0x????????:	or rz,rx,ry              	Rz=Rx|Ry=0x????????,SR=0x????????
			sprintf(instrucao, "or %s,%s,%s", specifyRegL(z).c_str(),
					specifyRegL(x).c_str(), specifyRegL(y).c_str());
			// Formatacao de saida em tela (deve mudar para o arquivo de saida)
			fprintf(write, "0x%08X:\t%-25s\t%s=%s|%s=0x%08X,SR=0x%08X\n", pc,
					instrucao, specifyRegU(z).c_str(), specifyRegU(x).c_str(),
					specifyRegU(y).c_str(), R[z], R[31]);

			break;

			// not
		case 0b001000:

			pc = R[29];
			x = (R[28] & (0b11111 << 16)) >> 16;
			y = (R[28] & (0b11111 << 11)) >> 11;
			z = (R[28] & (0b11111 << 21)) >> 21;

			R[z] = ~R[x];

			if (R[z] == 0) {
				setZN(R[31], true);
			} else {
				setZN(R[31], false);
			}

			if (checkBit(R[z], 31)) {
				setSN(R[31], true);
			} else {
				setSN(R[31], false);
			}

			//	0x????????:	not rz,rx                	Rz=~Rx=0x????????,SR=0x????????
			sprintf(instrucao, "not %s,%s", specifyRegL(z).c_str(),
					specifyRegL(x).c_str());
			// Formatacao de saida em tela (deve mudar para o arquivo de saida)
			fprintf(write, "0x%08X:\t%-25s\t%s=~%s=0x%08X,SR=0x%08X\n", pc,
					instrucao, specifyRegU(z).c_str(), specifyRegU(x).c_str(),
					R[z], R[31]);

			break;

			// xor
		case 0b001001:

			pc = R[29];
			x = (R[28] & (0b11111 << 16)) >> 16;
			y = (R[28] & (0b11111 << 11)) >> 11;
			z = (R[28] & (0b11111 << 21)) >> 21;

			R[z] = R[x] ^ R[y];

			if (R[z] == 0) {
				setZN(R[31], true);
			} else {
				setZN(R[31], false);
			}

			if (checkBit(R[z], 31)) {
				setSN(R[31], true);
			} else {
				setSN(R[31], false);
			}

			//	0x????????:	xor rz,rx,ry             	Rz=Rx^Ry=0x????????,SR=0x????????
			sprintf(instrucao, "xor %s,%s,%s", specifyRegL(z).c_str(),
					specifyRegL(x).c_str(), specifyRegL(y).c_str());
			// Formatacao de saida em tela (deve mudar para o arquivo de saida)
			fprintf(write, "0x%08X:\t%-25s\t%s=%s^%s=0x%08X,SR=0x%08X\n", pc,
					instrucao, specifyRegU(z).c_str(), specifyRegU(x).c_str(),
					specifyRegU(y).c_str(), R[z], R[31]);

			break;
			// Instrucao desconhecida
		default:
			// Exibindo mensagem de erro
			printf("Instrucao desconhecida!\n");
			// Parar a execucao
			return 0;
			execution = false;
			break;
		}

		R[29] = R[29] + 4;
		i++;
	}

	fprintf(write, "[END OF SIMULATION]\n");
	fclose(write);
	return 0;
}

