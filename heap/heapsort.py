import matplotlib.pyplot as plt
import csv

sizes = []
heapSort_times = []
std_sort_times = []

with open('sort_times.csv', 'r') as file:
    reader = csv.reader(file)
    next(reader)  # 跳过表头
    for row in reader:
        sizes.append(int(row[0]))
        heapSort_times.append(float(row[1]))
        std_sort_times.append(float(row[2]))

plt.plot(sizes, heapSort_times, label='HeapSort')
plt.plot(sizes, std_sort_times, label='std::sort')
plt.xlabel('Input Size')
plt.ylabel('Time (ms)')
plt.title('HeapSort vs std::sort Time Complexity')
plt.legend()
plt.xscale('log')
plt.yscale('log')
plt.show()
