#include "console.h"
#include "flags.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usbd_comp_if.h"

#define MAXTERM	256
enum ConsoleState {NOT_CONN, USER_PROMPT, PASS_PROMPT, CMD_LINE};

enum ConsoleState term_state;
char term_line[MAXTERM];
uint8_t term_idx=0;
uint8_t right_user=0;
uint8_t tries=0;

void sendLine(char* line) {
	uint8_t l = strlen(line);
	while (CDC_Transmit_FS(line, l) == USBD_BUSY)
		HAL_Delay(10);
}

void sendChar(char c) {
	while (CDC_Transmit_FS(&c, 1) == USBD_BUSY)
		HAL_Delay(10);
}

void consoleConnect() {
	term_state=NOT_CONN;
	memset(term_line, 0, MAXTERM);
	term_idx=0;
}

void sendBanner() {
	term_state = USER_PROMPT;
	sendLine("Ultrac Electric MX-2600\r\nlogin: ");
}

void handleTermLine() {
	switch(term_state) {
	case NOT_CONN:
		if (checkFlag(FLAG_PANEL) && checkFlag(FLAG_VOLT)) {
			sendBanner();
		}
		else
			sendLine("Nothing to connect to... yet\r\n");
		break;
	case USER_PROMPT:
		if (strlen(term_line) == 0) {
			sendBanner();
			break;
		}
		right_user = strcasecmp(term_line, "admin")==0;
		sendLine("password: ");
		term_state = PASS_PROMPT;
		break;
	case PASS_PROMPT:
		if ((right_user) && (strcmp(term_line, "charleM!800")==0)) {
			sendLine("$ ");
			term_state = CMD_LINE;
		}
		else {
			sendLine("Invalid credentials\r\n\n");
			consoleConnect();
		}
		break;
	case CMD_LINE:
		{
			char* item=strtok(term_line, " ");
			if (strcasecmp(item, "quit") == 0) {
				sendLine("Goodbye!\r\n\r\n");
				sendBanner();
				return;
			}
			else if (strcasecmp(item, "clear") == 0) {
				sendLine("\033[2J\r\n");
			}
			else if (strcasecmp(item, "help") == 0) {
				sendLine("Commands: clear help quit set ssh\r\n");
			}
			else if (strcasecmp(item, "ssh") == 0) {
				item = strtok(NULL, " ");
				if (strcasecmp(item, "enable")==0)
					sendLine("SSH access enabled\r\n");
				else if (strcasecmp(item, "disable")==0)
					sendLine("SSH access disabled\r\n");
				else
					sendLine("invalid SSH value\r\n");
			}
			else if (strcasecmp(item, "set") == 0) {
				item = strtok(NULL, " ");
				if (strcasecmp(item, "threshold")==0) {
					item = strtok(NULL, " ");
					int v = atoi(item);
					if (v) {
						sendLine("Voltage threshold changed from 5000 Volts to ");
						sendLine(item);
						sendLine(" Volts\r\n");
						if (v == 1000) {
							setFlag(FLAG_CONSOLE);
							sendLine("Bert says: \"Well look at that! No wonder these have been blowing apart. I'll see if I can script that up.\"\r\n");
						}
					}
					else
						sendLine("invalid threshold value");
				}
				else {
					sendLine("invalid set field '");
					sendLine(item);
					sendLine("'\r\n");
				}
			}
			else {
				sendLine("Invalid command\n");
			}
			sendLine("$ ");
			break;
		}
	}
	memset(term_line, 0, MAXTERM);
}

void consoleChar(char c) {
	if (c == '\r') {
		sendLine("\r\n");
		handleTermLine();
		term_idx=0;
	}
	else if (c == 8) {
		if (term_idx) {
			term_line[--term_idx] = 0;
			sendLine("\010");
		}
	}
	else if (c < ' ') {}
	else if (c > '~') {}
	else {
		if (term_state == PASS_PROMPT)
			sendChar('*');
		else
			sendChar(c);
		term_line[term_idx++]=c;
		if (term_idx >= MAXTERM)
			term_idx = MAXTERM-1;
	}
}

void consoleDisconnect() {

}

