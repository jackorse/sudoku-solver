import subprocess
import matplotlib.pyplot as plt

# Max number of usable threads
MAX_THREADS = 48
THREADS = ["1"] + [str(i) for i in range(2, MAX_THREADS+1, 2)]
# Sudoku board field sizes to test
FIELD_SIZES = [3,4,5]
# Sudoku board files to test
FILES = ["bin/sudoku-9x9.txt", "bin/sudoku-16x16.txt", "bin/sudoku-25x25.txt"]

# Array to store the data produced by the executions of the sudoku binary
data = []

# Execute the sudoku binary for each field size and each number of threads
for i in range(len(FIELD_SIZES)):
    for thread in THREADS:
        # Obtain the output of the sudoku binary in the form of a byte array
        output_bin_array = subprocess.run(["./bin/sudoku", str(FIELD_SIZES[i]*FIELD_SIZES[i]), str(FIELD_SIZES[i]), FILES[i]], env={"OMP_NUM_THREADS": thread}, capture_output=True)
        # Transform the byte array into a string
        output = output_bin_array.stdout.decode('utf-8')
        # Take the time taken to solve the sudoku board from the output string
        seconds = output.split("took ")[1].split(" seconds")[0]
        # Append the array made of the file name, the number of threads and the time taken to solve the sudoku board with the
        # current configuration [file, num_threads, time] to the data array
        data.append([FILES[i] ,thread, float(seconds)])
        print(f"File: {FILES[i]}, Threads: {thread}, {seconds} seconds")

# Arrays to store the computation times for each sudoku board size
computation_times_9x9 = []
computation_times_16x16 = []
computation_times_25x25 = []

# Fill the computation times arrays with the data from the data array according to the input file used: [9x9, 16x16, 25x25]
for d in data:
  if d[0] == FILES[0]:
    computation_times_9x9.append(d[2])
  if d[0] == FILES[1]:
    computation_times_16x16.append(d[2])
  if d[0] == FILES[2]:
    computation_times_25x25.append(d[2])

# Grouping the computation times arrays in a single array
computation_times = [computation_times_9x9, computation_times_16x16, computation_times_25x25]

# Plotting the computation times for each sudoku board size in different plots
for i in range(len(computation_times)):
  plt.figure()
  # Plot as x-axis the number of threads and as y-axis the computation time
  plt.plot(THREADS, computation_times[i])

  # Set the labels, title and legend of the plot
  plt.xlabel("Threads")
  plt.ylabel("Time [s]")
  board_size = FILES[i].split("-")[1].split(".")[0]
  plt.legend([board_size])
  plt.title('Performance')
  
  # Save the plot as a png file
  plt.savefig(f"plot{board_size}.png")
  plt.close()



