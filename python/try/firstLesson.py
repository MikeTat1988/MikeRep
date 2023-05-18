# Example code using print, del, def, and pass

# Function that prints a welcome message
def greet(string):
    print(string)

# Function that does nothing
def dummy_function():
    pass

# Main program
name = "John"
age = 25
string = f"My name is {name} and I am {age} years old"

greet(string)

# Deleting the name variable
del name

def diffOrder(age, name = "Default"):
    print("Hi, my name is "+name+ " and my age is " + str(age))

def my_function(num : int)->int:
    if isinstance(num,int):
        print("is int")
        return "bla"
    else:
        raise TypeError("arguments must be integers")

# Output: No parameter provided.

# Calling the function with a parameter
a = my_function(42)
print(a)

diffOrder(age = 31 )
# my_function("ALICE")
