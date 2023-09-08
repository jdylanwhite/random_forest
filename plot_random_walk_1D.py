# Load libraries
import pandas as pd
import matplotlib.pyplot as plt

# Read location data
inputFile = 'location.csv'
df = pd.read_csv(inputFile)

# Initialize the figure
fig, ax = plt.subplots(figsize=(8,6))

# Plot the data
x = df['i'].to_list()
y = df['location'].to_list()
ax.plot(x,[0 for j in y],'k-')
ax.plot(x,y,'b-')

# Set the x and y axis limits
xMin = x[0]
xMax = x[-1]
yMin = -max([abs(i) for i in y])
yMax = -yMin
ax.set_xlim(xMin,xMax)
ax.set_ylim(yMin*1.1,yMax*1.1)
    
# Label the axes
ax.set_title('Random Walk Location',fontsize=18)
ax.set_xlabel('Step Count',fontsize=14)
ax.set_ylabel('Location',fontsize=14)
plt.xticks(fontsize=12)
plt.yticks(fontsize=12)

# Save plot 
fig.savefig('location.png',dpi=100)

print('Done')