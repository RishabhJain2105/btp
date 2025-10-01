import random
import sys

def main():
    # Open the file for writing
    with open("time.txt", "w") as file:
        for _ in range(int(sys.argv[1])):  # Adjust count for more values
            random_value = random.randint(1, 5)  # Random number between 1 and 5
            file.write(f"{random_value}\n")

    print("Random values written to time.txt")

if __name__ == "__main__":
    main()
