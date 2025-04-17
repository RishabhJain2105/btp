import matplotlib.pyplot as plt

# Read latency values from file
with open('proactive_latency30.txt', 'r') as file:
    latency_values = [float(line.strip()) for line in file.readlines()]

# Generate x-values (1 to 90)
x_values = list(range(1, len(latency_values) + 1))

# Plotting
plt.figure(figsize=(10, 6))  # Optional: Adjust figure size
plt.plot(x_values, latency_values, marker='o', linestyle='-', color='b', label='Latency')
plt.xlabel('Number of gNBs')
plt.ylabel('Latency')
plt.title('Latency vs gNB')
plt.grid(True)
plt.legend()
plt.tight_layout()

# Save the plot as a file (optional)
plt.savefig('latency_vs_gnb_proactive.png')

# Show the plot
plt.show()
'''
# Read latency values from file
with open('proactive_latency24.txt', 'r') as file:
    latency_values = [float(line.strip()) for line in file.readlines()]

# Generate x-values (1 to 90)
x_values = list(range(1, len(latency_values) + 1))

# Plotting
plt.figure(figsize=(10, 6))  # Optional: Adjust figure size
plt.plot(x_values, latency_values, marker='o', linestyle='-', color='b', label='Latency')
plt.xlabel('Number of gNBs')
plt.ylabel('Latency')
plt.title('Latency vs gNB')
plt.grid(True)
plt.legend()
plt.tight_layout()

# Save the plot as a file (optional)
plt.savefig('latency_vs_gnb_proactive 24.png')

# Show the plot
#plt.show()

# Read latency values from file
with open('proactive_latency27.txt', 'r') as file:
    latency_values = [float(line.strip()) for line in file.readlines()]

# Generate x-values (1 to 90)
x_values = list(range(1, len(latency_values) + 1))

# Plotting
plt.figure(figsize=(10, 6))  # Optional: Adjust figure size
plt.plot(x_values, latency_values, marker='o', linestyle='-', color='b', label='Latency')
plt.xlabel('Number of gNBs')
plt.ylabel('Latency')
plt.title('Latency vs gNB')
plt.grid(True)
plt.legend()
plt.tight_layout()

# Save the plot as a file (optional)
plt.savefig('latency_vs_gnb_proactive 27.png')

# Show the plot
#plt.show()

# Read latency values from file
with open('proactive_latency30.txt', 'r') as file:
    latency_values = [float(line.strip()) for line in file.readlines()]

# Generate x-values (1 to 90)
x_values = list(range(1, len(latency_values) + 1))

# Plotting
plt.figure(figsize=(10, 6))  # Optional: Adjust figure size
plt.plot(x_values, latency_values, marker='o', linestyle='-', color='b', label='Latency')
plt.xlabel('Number of gNBs')
plt.ylabel('Latency')
plt.title('Latency vs gNB')
plt.grid(True)
plt.legend()
plt.tight_layout()

# Save the plot as a file (optional)
plt.savefig('latency_vs_gnb_proactive 30.png')

# Show the plot
#plt.show()

# Read latency values from file

with open('reactive_latency.txt', 'r') as file:
    latency_values = [float(line.strip()) for line in file.readlines()]

# Generate x-values (1 to 90)
x_values = list(range(1, len(latency_values) + 1))

# Plotting
plt.figure(figsize=(10, 6))  # Optional: Adjust figure size
plt.plot(x_values, latency_values, marker='o', linestyle='-', color='b', label='Latency')
plt.xlabel('Number of gNBs')
plt.ylabel('Latency')
plt.title('Latency vs gNB')
plt.grid(True)
plt.legend()
plt.tight_layout()

# Save the plot as a file (optional)
plt.savefig('latency_vs_gnb_reactive.png')

# Show the plot
plt.show()
'''
