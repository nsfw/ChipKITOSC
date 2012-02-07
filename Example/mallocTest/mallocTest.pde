
void testAlloc(unsigned int sz){
    void *ptr = malloc(sz);
    Serial.print("alloc: ");
    Serial.println(sz);
    Serial.print(" @ ");
    Serial.println((unsigned int) ptr, HEX);
    free(ptr);
}


void setup(){ 
    Serial.begin(57600);
    Serial.println("Hello!!");
}
  
void loop(){
    static unsigned int p = 0x10;
    testAlloc(p);
    p+=15;
    delay(100);
    Serial.print(".");
}


