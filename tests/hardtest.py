#!/bin/python3
import sys

# sys.stdout.write("ls && ")
# for _ in range(1000):
#     sys.stdout.write("(ls && ")
# for _ in range(1000):
#     sys.stdout.write(")")
# sys.stdout.flush()

print("ls && ", end="")
for _ in range(10000):
    print("ls && ", end="")
print("ls", end="")
# for _ in range(1000):
#     print(")", end="")