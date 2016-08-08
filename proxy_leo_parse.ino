void setup() {
  // Open Serial to PC
  Serial.begin(115200);
  // Open Serial to GRBL
  Serial1.begin(115200);

  while (!Serial);
  while (!Serial1); 
}

const int BUFFER_SIZE = 70;
char pcserial[BUFFER_SIZE];
char grserial[BUFFER_SIZE];

int pc = 0;
int gr = 0;

void loop() {
  // Get data from GRBL ==> PC
  while(Serial1.available()!=0) {
    char c = Serial1.read();
   
    // wait for a complete line 
    // and parse it
    if(c == '\n'){
      parseGrblLine(grserial);
      gr = 0;
      memset(&grserial[0], 0, sizeof(grserial));
      grserial[0] = '\0';
    }
    else if(gr < BUFFER_SIZE){
      grserial[gr++] = c;
    }
   
    // dont send data from $G to Serial, 
    // cuz UGS don't understand this
    // dont send data if string empty
    if(grserial[0] != '['){
       Serial.print(c);
    }
  }

  // Get data from PC ==> GRBL
   while(Serial.available()!=0) {
    char c = Serial.read();
      
    // wait for a complete line
    // and parse it
    if(c == '\n'){
      parsePCCommand(pcserial);
      pc = 0;
      memset(&pcserial[0], 0, sizeof(pcserial));
      pcserial[0] = '\0';
      }
      else {
      // if to big ...
      if(pc < BUFFER_SIZE){
        pcserial[pc++] = c;
      }
    }
  }
}

// Analyze every line and choose an action
void parseGrblLine(char* grserial){
}

const int COORD_SIZE = 20; // Do I need COORD_SIZE or [] would do?

// Analyze every command (from PC => GRBL) and choose an action
void parsePCCommand(char* pcserial){
  char xchar[COORD_SIZE]; // Do I need COORD_SIZE or [] would do?
  char ychar[COORD_SIZE]; // Do I need COORD_SIZE or [] would do?
  char zchar[COORD_SIZE]; // Do I need COORD_SIZE or [] would do?
  int digit = 0;
  double xcoord;
  double ycoord;
  double zcoord;

  // Read coordinates
  for(int i=0;i<sizeof(pcserial)-1;i++){
    if(pcserial[i]=='X'){   // Only working for G0 instructions (relative motion)
      for(int j=i+1;j<sizeof(pcserial)-1;j++){
        if(pcserial[j]==' ' || pcserial[j]=='\0'){
          digit = 0;
          break;
        }
        xchar[digit++] = pcserial[j];
      }
      xcoord = atof(xchar);
    }
    if(pcserial[i]=='Y'){   // Only working for G0 instructions (relative motion)
      for(int j=i+1;j<sizeof(pcserial)-1;j++){
        if(pcserial[j]==' ' || pcserial[j]=='\0'){
          digit = 0;
          break;
        }
        ychar[digit++] = pcserial[j];
      }
      ycoord = atof(ychar);
    }
    if(pcserial[i]=='Z'){   // Only working for G0 instructions (relative motion)
      for(int j=i+1;j<sizeof(pcserial)-1;j++){
        if(pcserial[j]==' ' || pcserial[j]=='\0'){
          digit = 0;
          break;
        }
        zchar[digit++] = pcserial[j];
      }
      zcoord = atof(zchar);
    }
  }
  
  // Next step:       Print on Serial (PC) xoord, ycoord and zcoord. Check if numbers show up ok.
  // Following step:  Manipulate coords to equivalent S1, S2 and S3 payed out lengths.
  //                  Compile back into "G" command.
  //                  Print on Serial1 to send to GRBL.
}
