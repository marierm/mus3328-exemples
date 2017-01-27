#define MESSAGE_BUFFER_SIZE 64
#define MESSAGE_BUFFER_LAST_ELEMENT 63
char messageState = 0;
char messageBufferSerialByte;
int messageBufferIndex = 0;
char messageBuffer[MESSAGE_BUFFER_SIZE];
char *messageBufferLast;
char *messageBufferWord;
char messageSendState = 0;
int messageBuild();
int messageGetInt();

int messageBuild() {
  int size = 0;
  messageState = 0;
  while (Serial.available() > 0) {
    messageBufferSerialByte = Serial.read();
    switch (messageBufferSerialByte) {
      case 10:
        break;
      case 13:
        size = messageBufferIndex;
        messageBuffer[messageBufferIndex]=0;
        messageBufferIndex=0;
        messageState = 1;
        break;

    default:
      messageBuffer[messageBufferIndex]=messageBufferSerialByte;
      messageBufferIndex = messageBufferIndex + 1;
    }
    if (messageBufferIndex >= MESSAGE_BUFFER_LAST_ELEMENT) messageBufferIndex=0;
  }
  return size;
}


int messageGetInt() {
    if (messageNext()) return atoi(messageBufferWord);
  return 0;
}

char messageNext() {
  char * temppointer= NULL;
  switch (messageState) {
  case 0:
    return 0;
  case 1:
    temppointer = messageBuffer;
    messageState = 2;
  default:
    messageBufferWord = strtok_r(temppointer," ",&messageBufferLast);
    if (messageBufferWord != NULL) return 1;
  }
  return 0;
}
