class Employee{
String lastname;
String firstname;
String title;
boolean status;
void showAttributes(){
System.out.println("Employee name: " + firstname + " " + lastname);
System.out.println("Employee title: " + title);
if (status == true)
System.out.println("Employee status = active");
else
System.out.println("Employee status = inactive");
}
public static void main (String arguments[]){
Employee e = new Employee();
e.lastname = "Smith";
e.firstname = "Joe";
e.title = "Finance";
e.status = true;
e.showAttributes();
}
}
