import subprocess
import matplotlib.pyplot as plt

THREADS = ["1","2","4","6","8","10","12","14","16","18","20","22","24","26","28",
           "30","32","34","36","38","40","42","44","46","48"]
FIELD_SIZES = [3,4,5]
FILES = ["bin/sudoku-9x9.txt", "bin/sudoku-16x16.txt", "bin/sudoku-25x25.txt"]

data = []
for i in range(len(FIELD_SIZES)):
    for thread in THREADS:
        output = subprocess.run(["./sudoku", str(FIELD_SIZES[i]*FIELD_SIZES[i]), str(FIELD_SIZES[i]), FILES[i]], env={"OMP_NUM_THREADS": thread}, capture_output=True)
        output = output.stdout.decode('utf-8')
        line_sec = output.split("\n")[-4]
        seconds = line_sec.split("took ")[1].split(" seconds")[0]
        data.append([FILES[i] ,thread, float(seconds)])
        print(f"File: {FILES[i]}, Threads: {thread}, {seconds} seconds")

num_of_threads = []
computation_times_9x9 = []
computation_times_16x16 = []
computation_times_25x25 = []
for d in data:
  if d[0] == FILES[0]:
    computation_times_9x9.append(d[2])
  if d[0] == FILES[1]:
    computation_times_16x16.append(d[2])
  if d[0] == FILES[2]:
    computation_times_25x25.append(d[2])



plt.figure()
plt.plot(THREADS, computation_times_9x9)
plt.xlabel("Threads")
plt.ylabel("Time [s]")
plt.legend(["9x9"])
plt.title('Performances')
plt.savefig("plot9x9.png")
plt.close()

plt.figure()
plt.plot(THREADS, computation_times_16x16)
plt.xlabel("Threads")
plt.ylabel("Time [s]")
plt.legend(["16x16"])
plt.title('Performances')
plt.savefig("plot16x16.png")
plt.close()

plt.figure()
plot25x25 = plt.plot(THREADS, computation_times_25x25)
plt.xlabel("Threads")
plt.ylabel("Time [s]")
plt.legend(["25x25"])
plt.title('Performances')
plt.savefig("plot25x25.png")
plt.close()



