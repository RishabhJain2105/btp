import matplotlib.pyplot as plt

x = [0, 4, 5, 9, 10, 12, 15, 18, 21, 22, 26, 27, 29, 30, 31, 32, 35, 39, 42, 44, 46, 50, 51, 54, 56, 57, 59, 63, 66, 69, 72, 73, 75, 78, 79, 82, 86, 89, 90, 92, 95, 99, 102, 106, 110, 113, 114, 117, 119, 122, 126, 129, 131, 135, 137, 140, 144, 147, 149, 151, 152, 153, 156, 159, 162, 165, 166, 169, 170, 171, 175, 178, 179, 181, 184, 188, 192, 195, 197, 199, 200]
y = [1] * 30 + [2] * 30 + [3] * 21

plt.figure(figsize=(10, 5))
plt.step(x, y, where='post', linestyle='-', color='b', marker='o')
plt.xlabel('Time (seconds)')
plt.ylabel('Number of AMFs')
plt.title('Number of AMFs over Time')
plt.grid(True)
plt.text(177, 2.5, 'gnb_count = 61', horizontalalignment='center')
plt.text(97, 1.5, 'gnb_count = 31', horizontalalignment='center')
plt.yticks([1, 2, 3])
plt.savefig('reactive_amf_time.png')
