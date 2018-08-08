import os
import sys


def main(args):
    if len(args) != 1:
        print("Usage: %s <report file>" % os.path.basename(sys.argv[0]))
        return 1
    numbers = []
    for line in open(args[0]):
        if line and line[0] != "/":
            numbers.extend(int(n) for n in line.split())

    start_index = 0
    for i, n in enumerate(numbers):
        if n > 90:
            start_index = i
            break
    else:
        print("All durations are less than 90 ms")
        return 0

    accumulated_duration = numbers[start_index]
    for i in range(start_index + 1, len(numbers)):
        if numbers[i] > 90:
            print("%d values reported, %d ms" % (i - start_index,
                                                 accumulated_duration))
            for j in range(start_index, i):
                print("%3d" % numbers[j], end=" ")
                if (j - start_index + 1) % 16 == 0:
                    print()
            print()
            accumulated_duration = numbers[i]
            start_index = i
        else:
            accumulated_duration += numbers[i]
    return 0


sys.exit(main(sys.argv[1:]))
