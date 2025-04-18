import sys
import matplotlib.pyplot as plt

def main(n):
    # Read time values from time.txt
    with open('time.txt', 'r') as file:
        x_values = [int(line.strip()) for line in file.readlines()]
    
    # Generate y values based on n
    y_values = [1] * n + [2] * n + [3] * (len(x_values) - 2 * n)
    
    # Plot the values
    plt.figure()
    
    plt.plot(x_values[:n], y_values[:n])
    plt.plot(x_values[n:2*n], y_values[n:2*n])
    plt.plot(x_values[2*n:], y_values[2*n:])
    
    # Draw lines and annotate with gnb_count
    plt.plot([x_values[n-1], x_values[n]], [1, 2], 'k--')
    plt.text((x_values[n-1] + x_values[n]) / 2 + 35, 1.5, f'gnb_count = {n}', ha='center')
    
    plt.plot([x_values[2*n-1], x_values[2*n]], [2, 3], 'k--')
    plt.text((x_values[2*n-1] + x_values[2*n]) / 2 + 35, 2.5, f'gnb_count = {2*n}', ha='center')
    
    # Set titles and labels
    plt.title('AMF vs Time')
    plt.xlabel('Time')
    plt.ylabel('AMF')
    
    # Set y-axis ticks
    plt.yticks([1, 2, 3])
    
    # Show legend
    plt.legend()
    
    # Save the plot before showing it
    plt.savefig('AMF_vs_Time30.png')
    
    # Show the plot
    plt.show()

if __name__ == '__main__':
    
    if len(sys.argv) != 2:
        print("Usage: python3 plot.py n")
        sys.exit(1)
        
    n = int(sys.argv[1])
    main(n)

