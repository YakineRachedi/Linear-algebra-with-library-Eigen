import matplotlib.pyplot as plt

def read_data(filename):
    x = []
    y = []
    with open(filename, 'r') as file:
        for line in file:
            parts = line.split()
            x.append(float(parts[0]))
            y.append(float(parts[1]))
    return x, y

# Read data from the two files
x1, y1 = read_data('eigenvalues.dat')
x2, y2 = read_data('EigenValues_class.dat')

# Create a figure with two side-by-side subplots
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 6))

# First subplot
ax1.bar(x1, y1, width=(x1[1] - x1[0]), align='center', edgecolor='black',color = 'red')
ax1.set_xlabel('Eigenvalue')
ax1.set_ylabel('Frequency')
ax1.set_title('Histogram of Eigenvalues (eigenvalues.dat)')
# Adjust the y-axis range if necessary for the first subplot
ax1.set_ylim(0, max(y1) * 1.1)

# Second subplot
ax2.bar(x2, y2, width=(x2[1] - x2[0]), align='center', edgecolor='black')
ax2.set_xlabel('Eigenvalue')
ax2.set_ylabel('Frequency')
ax2.set_title('Histogram of Eigenvalues (EigenValues_class.dat)')

# Adjust the y-axis range if necessary for the second subplot
ax2.set_ylim(0, max(y2) * 1.1)

# Adjust spacing between subplots
plt.tight_layout()

# Display and save the graphs
plt.savefig("Eigenvalues.png")
plt.show()