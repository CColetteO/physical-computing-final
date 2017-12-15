int teaNum = 0; 
const int oneTime = 140;
const int rot45 = 180; 
int cups = 0;
int cupsPer = 4; 

int low = D5; 
int high = D6; 
int dispense = D4; //(This is the button to dispense, not the dispenser itself)
int refill = D7; 
int servoSel = D0; 
int servoDis = D3; 

Servo selector; 
Servo dispenser; 

//Fill Trackers
int Sun = 0; 
int Mon = 0; 
int Tue = 0; 
int Wed = 0; 
int Thu = 0; 
int Fri = 0; 
int Sat = 0; 

void setup() {
    //Assign pins in/out
    pinMode(low, INPUT_PULLUP); 
    pinMode(high, INPUT_PULLUP);
    pinMode(dispense, INPUT_PULLUP);
    pinMode(refill, INPUT_PULLUP); 
    selector.attach(servoSel); 
    dispenser.attach(servoDis); 
    
    //Public functions/variables
    Particle.variable("TeaType", &teaNum, INT);
    Particle.variable("CupsSaved", &cups, INT);
    Particle.variable("DailyAmount", &cupsPer, INT); 
    //Particle.function("ChangeTea", remoteTurn); 
    Particle.function("DispenseTea", remoteDispense);
    //Particle.function("SelectRandom", remoteRandomize);
    Particle.variable("Sun", &Sun, INT); 
    Particle.variable("Mon", &Mon, INT); 
    Particle.variable("Tue", &Tue, INT); 
    Particle.variable("Wed", &Wed, INT); 
    Particle.variable("Thu", &Thu, INT); 
    Particle.variable("Fri", &Fri, INT); 
    Particle.variable("Sat", &Sat, INT); 
    Particle.function("ChangeLow", remoteLow); 
    Particle.function("ChangeHigh", remoteHigh);
    
}

void loop() {
    int lowState; 
    int highState; 
    int dispState; 
    int fillState; 
    
    lowState = digitalRead(low); 
    highState = digitalRead(high); 
    dispState = digitalRead(dispense); 
    fillState = digitalRead(refill); 
    
    if (lowState == LOW) { 
        turnLow(); 
    } else if (highState == LOW) { //elif since shouldn't be trying to turn left and right at the same time
        turnHigh(); 
    } 
    if (fillState == LOW) { 
        setFull(); 
    } //Want to set as full before emptying zeroes it, if there's a conflict! 
    if (dispState == LOW) { 
        dispenseTea(); 
    }
    delay(oneTime*1.5); 
}


int dispenseTea() { 
    dispenser.write(30); //forward
    delay(2*rot45); //rotate just over 90deg
    dispenser.write(85); //stop
    delay(4000); //wait to dispense
    dispenser.write(130); //back
    delay(rot45); //rotate back just over 90deg
    dispenser.write(90); //stop
    dispenser.write(40); //forward
    delay(2.2*rot45); //rotate just over 90deg
    dispenser.write(85); //stop
    delay(2000); //wait to dispense
    dispenser.write(130); //back
    delay(rot45); //rotate back just over 90deg
    dispenser.write(90); //stop
    dispenser.write(40); //forward
    delay(2.2*rot45); //rotate just over 90deg
    dispenser.write(85); //stop
    delay(10000); //wait to dispense
    dispenser.write(140); //back
    delay(1.27*rot45); //rotate back just over 90deg
    dispenser.write(90); //stop
    
    cups = cups + cupsPer; 
    setEmpty(); 
    return 1; 
}

void setEmpty() {
    if (teaNum == 1) { 
        Sun = 0; 
    } else if (teaNum == 2) {
        Mon = 0; 
    } else if (teaNum == 3) {
        Tue = 0; 
    } else if (teaNum == 4) {
        Wed = 0; 
    } else if (teaNum == 5) {
        Thu = 0; 
    } else if (teaNum == 6) {
        Fri = 0; 
    } else if (teaNum == 7) {
        Sat = 0; 
    } //otherwise it's 0 and someone was silly, or there's a bug and teaNum is out of range
}

void setFull() { 
    if (teaNum == 1) { 
        Sun = 1; 
    } else if (teaNum == 2) {
        Mon = 1; 
    } else if (teaNum == 3) {
        Tue = 1; 
    } else if (teaNum == 4) {
        Wed = 1; 
    } else if (teaNum == 5) {
        Thu = 1; 
    } else if (teaNum == 6) {
        Fri = 1; 
    } else if (teaNum == 7) {
        Sat = 1; 
    } //otherwise it's 0 and someone was silly, or there's a bug and teaNum is out of range
}
    

void turnLown(int n) {
    teaNum = teaNum - n; 
    //teaNum = teaNum % 8; % causing odd bugs, so doing it the hard way 
        while (teaNum < 0) {
        teaNum = teaNum + 8;
    }
    selector.write(30); 
    delay(oneTime*n); 
    selector.write(90); 
}

void turnHighn(int n) {
    teaNum = teaNum + n; 
    //teaNum = teaNum % 8; % causing odd bugs, so doing it the hard way
    while (teaNum >= 8) {
        teaNum = teaNum - 8;
    }
    selector.write(150); 
    delay(oneTime*n); 
    selector.write(90); 
}


void turnLow() {
    turnLown(1); 
}

void turnHigh() {
    turnHighn(1); 
}


int remoteDispense(String s){ 
    return dispenseTea(); 
}


int remoteLow(String s) {
    turnLow(); 
    delay(oneTime*1.5); 
    return 1;
}

int remoteHigh(String s) {
    turnHigh(); 
    delay(oneTime*1.5); 
    return 1; 
}
