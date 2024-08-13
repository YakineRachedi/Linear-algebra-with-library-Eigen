import matplotlib.pyplot as plt

# Read data from the file
filename = 'eigenvalues.dat'
x = []
y = []

with open(filename, 'r') as file:
    for line in file:
        parts = line.split()
        x.append(float(parts[0]))
        y.append(float(parts[1]))

# Create the graph
plt.bar(x, y, width=(x[1] - x[0]), align='center', edgecolor='black')

# Add labels and a title
plt.xlabel('Eigenvalue')
plt.ylabel('Frequency')
plt.title('Histogram of Eigenvalues')

# Adjust the range of the ordinates if necessary
plt.ylim(0, max(y) * 1.1)

# Save the picture
plt.savefig("EigenvaluesHistogram.png")
# Display the graph
plt.show()