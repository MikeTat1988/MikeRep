def validate_input():
    valid_input = False
    possible_num_inputs = list(range(1,10))
    numbers_dict = {
    "one": 1,
    "two": 2,
    "three": 3,
    "four": 4,
    "five": 5,
    "six": 6,
    "seven": 7,
    "eight": 8,
    "nine": 9,
    "ten": 10}
    while not valid_input:
        user_input = input('Enter a number 1-10 either text or digit')
        if user_input.isdigit() and int(user_input) in range(1, 11):
            valid_input = True
        elif user_input.lower() in numbers_dict:
            user_input = numbers_dict[user_input]
            valid_input = True
        else:
            print('invalid input, please enter valid one')
        
    return user_input
