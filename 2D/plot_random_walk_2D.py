# Import modules
import pandas as pd
import matplotlib.pyplot as plt

# Load data from output file
df = pd.read_csv('2D/location.csv')

# Set up the figure
fig, ax = plt.subplots(figsize=(8,6))

# Get the number of points and time steps
nt = df['n'].max()
nw = df['i'].max()

# Loop through data and plot data
for n in range(nt):

    # Only get data for this time step
    dfPlot = df.loc[df['n']==n]
    x = dfPlot['x'].to_list()
    y = dfPlot['y'].to_list()

    # Plot the data
    ax.plot(x,y,'.')

    # Set titles
    ax.set_title(f"Time: {n}")
    ax.set_xlabel('x')
    ax.set_ylabel('y')

    # Set axis limits
    ax.set_xlim(-1, 100)
    ax.set_ylim(-1, 100)
    
    # Pause the plot for a moment
    plt.pause(0.1)

    # Clear the plot to update the results
    if n != nt:
        ax.clear()
        
plt.show()

print("Done!")