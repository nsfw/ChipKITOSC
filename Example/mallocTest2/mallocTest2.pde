
void setup()
{
  Serial.begin(115200);
  Serial.println("Hello");
}

#define ALLOCS 200
#define ALLOCSIZE 0x100

void loop()
{
  int scnt = 0;
  int fcnt = 0;
  
  for (int i = 0; i < ALLOCS; i++)
  {
    byte* pb = (byte*)malloc(ALLOCSIZE);
    if (pb == NULL)
    {
      Serial.print("F");
      fcnt++;
    }
    else
    {
      Serial.print("S");
      scnt++;
    }
  }
  
  Serial.print("Using an ALLOCSIZE of ");
  Serial.println(ALLOCSIZE);
  Serial.print("Successfully allocated ");
  Serial.print(scnt, DEC);
  Serial.print(" times. Total memory allocated ");
  Serial.print(scnt * ALLOCSIZE);
  Serial.println(" bytes.");
  
  Serial.print(fcnt, DEC);
  Serial.println(" Failures");
  
  delay(1000000);
}