class foo {
    int a;
    int b;
public:
    foo();
};

foo::foo(){
    a=1;
    b=2;
}

void setup(){ 
     foo myfoo;	// OK
     foo *kablooey = new foo();	// GENERATES LINK ERROR
}

void loop(){
}
