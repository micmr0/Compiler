# Compiler
Compiler build by c++, flex and bison

##What works
  * int, float
  * if, while
  * print, println
  * write
  * string
  * array


##Language specification

###declaring a variables:
```
float a;
int b;
float c = 5.7;
int d = 5.5;
string a = "Something";
```
###call a functions:
```
print(a);
print(a+b*3);
println();
print("Something");
```
###Arithmetics:
```
int a = 5 + 5 * 3 - 7;
float b = 5.5 + 3 - 12 * 3.3;
```
###Loops:

```
while(1)
{
    ...
}

if(a == 5)
{
    ...
}
else
{
    ...
}
```
###Array
```
int @tab[5];
float @tab1[5];
@tab[0] = 5;
@tab[1] = 5.2;
@tab[2] = 5.4;
```
##How to use

Simple compile by writing: 
* make 
* ./leks < ../tests/test_name.txt
