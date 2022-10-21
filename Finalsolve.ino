//defining motor pins
int rm1 = 7;
int rm2 = 8;
int lm1 = 9;
int lm2 = 10;

//defining enable pins
int ep1 = 5;
int ep2 = 6;

int leftNudge;
int replaystage;
int rightNudge;

//speed of motors by pwm
//const int r_pwm = 180;
//const int l_pwm = 180;

char path[50] = {};
int pathLength;
int readLength;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //IR pins
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  //Motor pins
  pinMode(rm1, OUTPUT);
  pinMode(rm2, OUTPUT);
  pinMode(lm1, OUTPUT);
  pinMode(lm2, OUTPUT);
  //Enables pins
  pinMode(ep1, OUTPUT);
  pinMode(ep2, OUTPUT);  
}

void forward() {
  digitalWrite(rm1, HIGH);
  digitalWrite(rm2, LOW);
  digitalWrite(lm1, HIGH);
  digitalWrite(lm2, LOW);
  analogWrite(ep1, 50);
  analogWrite(ep2, 50);

	path[pathLength] = 'S';
		pathLength++;
		if(path[pathLength-2] == 'B')
		{
			shortPath();
		}
}

void right_90() {
  digitalWrite(rm1, LOW);
  digitalWrite(rm2, LOW);
  digitalWrite(lm1, HIGH);
  digitalWrite(lm2, LOW);
  analogWrite(ep1, 40);
  analogWrite(ep2, 70);

  if(replaystage==0){
  path[pathLength]='R';
  // Serial.println("r");
  pathLength++;
  // Serial.print("Path length: ");
  // Serial.println(pathLength);

    if(path[pathLength-2]=='B'){
      // Serial.println("shortening path");
      shortPath();
    }
  }
}

void left_90() {
  digitalWrite(rm1, HIGH);
  digitalWrite(rm2, LOW);
  digitalWrite(lm1, LOW);
  digitalWrite(lm2, LOW);
  analogWrite(ep1, 70);
  analogWrite(ep2, 40);

 if(replaystage==0){
    path[pathLength]='L';
    //Serial.println("l");
    pathLength++;
    //Serial.print("Path length: ");
    //Serial.println(pathLength);
      if(path[pathLength-2]=='B'){
        //Serial.println("shortening path");
        shortPath();
      }
  }
}

void r_slight() {
  digitalWrite(rm1, LOW);
  digitalWrite(rm2, HIGH);
  digitalWrite(lm1, HIGH);
  digitalWrite(lm2, LOW);
  analogWrite(ep1, 0);
  analogWrite(ep2, 60);
}

void l_slight() {
  digitalWrite(rm1, HIGH);
  digitalWrite(rm2, LOW);
  digitalWrite(lm1, LOW);
  digitalWrite(lm2, HIGH);
  analogWrite(ep1, 60);
  analogWrite(ep2, 0);
}

void backward() {
  digitalWrite(rm1, LOW); 
  digitalWrite(rm2, HIGH);
  digitalWrite(lm1, LOW);
  digitalWrite(lm2, HIGH);
  analogWrite(ep1, 60);
  analogWrite(ep2, 60);
}

void forward_slight() {
  digitalWrite(rm1, HIGH);
  digitalWrite(rm2, LOW);
  digitalWrite(lm1, HIGH);
  digitalWrite(lm2, LOW);
  analogWrite(ep1, 50);
  analogWrite(ep2, 50);
  delay(200);
}

void backward_slight() {
  digitalWrite(rm1, LOW);
  digitalWrite(rm2, HIGH);
  digitalWrite(lm1, LOW);
  digitalWrite(lm2, HIGH);
  analogWrite(ep1, 40);
  analogWrite(ep2, 40);
  delay(200);
}

void turnaround(){
  // left_90();
  digitalWrite(rm1, HIGH);
  digitalWrite(rm2, LOW);
  digitalWrite(lm1, LOW);
  digitalWrite(lm2, LOW);
  delay(100);

  // left_90();
  digitalWrite(rm1, HIGH);
  digitalWrite(rm2, LOW);
  digitalWrite(lm1, LOW);
  digitalWrite(lm2, LOW);
  delay(100);

  path[pathLength]='B';
  pathLength++;
  // straight();

}

void pause() {
  digitalWrite(rm1, LOW);
  digitalWrite(rm2, LOW);
  digitalWrite(lm1, LOW);
  digitalWrite(lm2, LOW);

  replaystage=1;

  path[pathLength]='D';
  pathLength++;

  delay(1500);//for calling shortpath -- Final Run
  replay();
}


bool stop = false; 
int back_count = 0;

void readSensors(){
    int x0 = digitalRead(A0);
    int x1 = digitalRead(A1);
    int x2 = digitalRead(A2);
    int x3 = digitalRead(A3);
    int x4 = digitalRead(A4);
    int x5 = digitalRead(A5);
}

void loop() {
  if(stop) {
    pause();
  }
  else {
    // put your main code here, to run repeatedly:
    //bool turn = false;
    //IR sensor INPUTs
    int x0 = digitalRead(A0);
    int x1 = digitalRead(A1);
    int x2 = digitalRead(A2);
    int x3 = digitalRead(A3);
    int x4 = digitalRead(A4);
    int x5 = digitalRead(A5);

    Serial.print(x0);
    Serial.print(" ");
    Serial.print(x1);
    Serial.print(" ");
    Serial.print(x2);
    Serial.print(" ");
    Serial.print(x3);
    Serial.print(" ");
    Serial.print(x4);
    Serial.print(" ");
    Serial.print(x5);
    Serial.print("\n");

    //condition for moving straight
    if(x0==0 && x1==0 && (x2==1 || x3==1) && x4==0 && x5==0) {
      forward();
      delay(500);
    }
    //moving backwards all white
   else if(x0==0 && x1==0 && x2==0 && x3==0 && x4==0 && x5==0) {
       turnaround();
       delay(200);
     }

    // blindspot get back
    // else if(x0==0 && x1==0 && x2==0 && x3==0 && x4==1 && x5==0) {
    //   backward();
    //   delay(200);
    // }

    else if(x0==0 && x1==0 && x2==0 && x3==0 && x4==0 && x5==0){
      forward_slight();
      delay(100);
      if(x0==0 && x1==0 && x2==0 && x3==0 && x4==0 && x5==0){
         turnaround();
         delay(100);
      }
    }

    else if(x0==1 && x1==1 && x2==1 && x3==1 && (x4==1 || x5==1)){
      forward_slight();
      delay(100);
      if(x0==0 && x1==0 && x2==0 && x3==0 && (x4==0 || x5==0)){
        left_90();
      }
    }

     //for Z turn right
      else if(x0==0 && x1==0 && x2==1 && x3==1 && (x4==1 || x5==1)){
        delay(500);
        right_90();
        delay(500);

        delay(500);
        r_slight();
        delay(600);
      }
      
     //for Z turn left
     else if((x0==1 || x1==1) && x2==1 && x3==1 && x4==0 && x5==0){
        delay(500);
        left_90();
        delay(500);

        delay(500);
        l_slight();
        delay(600); 
      }
      
    //right turn 90 degree
    else if(x0==0 && x1==0 && x2==1 && x3==1 && (x4==1 || x5==1)) {
      //slightly ahead
      forward_slight();
      delay(200);
      //for straight part
      if(x0==0 && x1==0 && x2==1 && x3==1 && x4==0 && x5==0) {
        forward();
        delay(500);
      }
      //right 90 only
      else {
        backward_slight();
        right_90();
        delay(500);
      }
    }
    //left turn 90 degree
    else if((x0==1 || x1==1) && x2==1 && x3==1 && x4==0 && x5==0) {
      //slightly ahead
      forward_slight();
      delay(200);
      //for straight part
      if(x0==0 && x1==0 && x2==1 && x3==1 && x4==0 && x5==0) {
        left_90();
        delay(250);
      }
      //left 90 only
      else {
        backward_slight();
        left_90();
        delay(250);
      }
    }
    //right turn slight
    else if(x0==0 && x1==0 && (x2==1 || x3==1) && x4==1 && x5==0) {
      r_slight();
      delay(250);
    }
    //left turn slight
    else if(x0==0 && x1==1 && (x2==1 || x3==1 )&& x4==0 && x5==0 ) {
      l_slight();
      delay(250);
    }
    //right most white 
    else if(x0==0 && x1==0 && x2==0 && x3==0 && x4==0 && x5==1) {
      r_slight();
      delay(250);
    }
    //right mid white
    else if(x0==0 && x1==0 && x2==0 && x3==0 && x4==1 && x5==0) {
      r_slight();
      delay(250);
    }
    //left most white
    else if(x0==1 && x1==0 && x2==0 && x3==0 && x4==0 && x5==0) {
      l_slight();
      delay(250);
    }
    //left mid white
    else if(x0==0 && x1==1 && x2==0 && x3==0 && x4==0 && x5==0) {
      l_slight();
      delay(250);
    }
    //all white
    else if(x0==1 && x1==1 && x2==1 && x3==1 && x4==1 && x5==1) {
      //check for cross and T
      //slightly ahead
      forward_slight();
      //reached end point
      if(x0==1 && x1==1 && x2==1 && x3==1 && x4==1 && x5==1) {
        pause();
        stop = true;
      }
     
      //reached plus || left obtuse angle || reached T intersection || right obtuse angle
      else if((x0==0 && x1==0 && x2==1 && x3==1 && x4==0 && x5==0) || (x0==0 && x1==0 && x2==0 && x3==0 && ( x4==1 || x5==1)) || (x0==0 && x1==0 && x2==0 && x3==0 && x4==0 && x5==0) || ((x0==1 || x1==1) && x2==0 && x3==0 && x4==0 && x5==0)) {
        left_90();
        delay(300);
      }
    }
  }
}

//Shortest path code start here!!
void shortPath(){
 int shortDone=0;

  if(path[pathLength-3]=='L' && path[pathLength-1]=='R'){
    pathLength-=3;
    path[pathLength]='B';
    //Serial.println("test1");
    shortDone=1;
  }

  if(path[pathLength-3]=='L' && path[pathLength-1]=='S' && shortDone==0){
    pathLength-=3;
    path[pathLength]='R';
    //Serial.println("test2");
    shortDone=1;
  }

  if(path[pathLength-3]=='R' && path[pathLength-1]=='L' && shortDone==0){
    pathLength-=3;
    path[pathLength]='B';
    //Serial.println("test3");
    shortDone=1;
  }

  if(path[pathLength-3]=='S' && path[pathLength-1]=='L' && shortDone==0){
    pathLength-=3;
    path[pathLength]='R';
    //Serial.println("test4");
    shortDone=1;
  }

  if(path[pathLength-3]=='S' && path[pathLength-1]=='S' && shortDone==0){
    pathLength-=3;
    path[pathLength]='B';
    //Serial.println("test5");
    shortDone=1;
  }

    if(path[pathLength-3]=='L' && path[pathLength-1]=='L' && shortDone==0){
    pathLength-=3;
    path[pathLength]='S';
    //Serial.println("test6");
    shortDone=1;
  }

  path[pathLength+1]='D';
  path[pathLength+2]='D';
  pathLength++;

  //Serial.print("Path length: ");
  //Serial.println(pathLength);
  //printPath();
}


//Here it is printing path
void printPath(){
  Serial.println("----------------");
  int x;

  while(x<=pathLength){
  Serial.println(path[x]);
  x++;
  }
  Serial.println("-----------------");
}

void replay(){
  // readSensors();
    int x0 = digitalRead(A0);
    int x1 = digitalRead(A1);
    int x2 = digitalRead(A2);
    int x3 = digitalRead(A3);
    int x4 = digitalRead(A4);
    int x5 = digitalRead(A5);
  if(x0 ==0 && x1==0 && (x2==1 || x3==1) && x4==0 && x5==0){
    forward();
  }

  else{
    if(path[readLength]=='D'){
    digitalWrite(lm1, HIGH);
    digitalWrite(lm2, LOW);
    digitalWrite(rm1, HIGH);
    digitalWrite(rm2, LOW);
    delay(600);

    digitalWrite(lm1, LOW);
    digitalWrite(lm2, LOW);
    digitalWrite(rm1, LOW);
    digitalWrite(rm2, LOW);
    endMotion();
    }

    if(path[readLength]=='R'){
    digitalWrite(lm1, HIGH);
    digitalWrite(lm2, LOW);
    digitalWrite(rm1, HIGH);
    digitalWrite(rm2, LOW);

    delay(600);
    right_90();
    }

    if(path[readLength]=='L'){
    digitalWrite(lm1, HIGH);
    digitalWrite(lm2, LOW);
    digitalWrite(rm1, HIGH);
    digitalWrite(rm2, LOW);

    delay(600);
    left_90();
    }

    if(path[readLength]=='S'){
    digitalWrite(lm1, HIGH);
    digitalWrite(lm2, LOW);
    digitalWrite(rm1, HIGH);
    digitalWrite(rm2, LOW);

    delay(600);
    forward();
    }
    readLength++;
  }
  replay();
}


void endMotion(){
  endMotion();
}
