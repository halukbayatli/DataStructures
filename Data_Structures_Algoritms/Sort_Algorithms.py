# Haluk Bayatlı 231000110030
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

def InsertionSort(array):
    for step in range(1,len(array)):
        key = array[step]
        j = step - 1
        while j >= 0 and key < array[j]:
            array[j+1] = array[j]
            yield array
            j -= 1
        array[j+1] = key
        yield array
    
def RadixSort(array):
    maxVal = max(array)
    place = 1
    while maxVal // place > 0:
        yield from CountingSort_Radix(array, place)
        place *= 10

def CountingSort_Radix(array, place):
    size = len(array)
    countArray = [0] * 10
    R = [0] * size
    for item in array:
        index = item // place
        countArray[index % 10] += 1
    
    for i in range(1, 10):
        countArray[i] += countArray[i-1]

    for i in range(size-1, -1, -1):
        index = array[i] // place
        R[countArray[index % 10]-1] = array[i]
        countArray[index % 10] -= 1

    for i in range(size):
        array[i] = R[i]
        yield array

def HeapSort(array):
    n = len(array)
    for i in range(n//2 - 1, -1, -1):
        Heapify(array, i, n)

    for i in range(n-1, 0, -1):
        array[i], array[0] = array[0], array[i]
        yield array
        Heapify(array, 0, i)

def Heapify(array, i, n):
    largest = i
    left = 2 * i + 1
    right = 2 * i + 2

    if left < n and array[left] > array[largest]:
        largest = left
    if right < n and array[right] > array[largest]:
        largest = right
    if largest != i:
        array[i], array[largest] = array[largest], array[i]
        Heapify(array, largest, n)

def BubbleSort(array):
    for i in range(len(array)):
        for j in range(len(array) - 1 - i):
            if array[j] > array[j + 1]:
                array[j], array[j + 1] = array[j + 1], array[j]
                yield array

def SelectionSort(array):
    for i in range(len(array)):
        minIndex = i
        for j in range(i + 1, len(array)):
            if array[j] < array[minIndex]:
                minIndex = j
        array[i], array[minIndex] = array[minIndex], array[i]
        yield array

def MergeSort(Array):
    def sort_helper(Array, start, end):
        if end <= start:
            return
        mid = start+((end-start+1)//2)-1
        yield from sort_helper(Array, start, mid)
        yield from sort_helper(Array, mid + 1, end)
        yield from Merge(Array, start, mid, end)
    yield from sort_helper(Array, 0, len(Array) - 1)

def Merge(Array, start, mid, end):
    merged = []
    leftIndex = start
    rightIndex = mid+1

    while leftIndex <= mid and rightIndex <= end:
        if Array[leftIndex] < Array[rightIndex]:
            merged.append(Array[leftIndex])
            leftIndex += 1
        else:
            merged.append(Array[rightIndex])
            rightIndex += 1

    while leftIndex <= mid:
        merged.append(Array[leftIndex])
        leftIndex += 1

    while rightIndex <= end:
        merged.append(Array[rightIndex])
        rightIndex += 1

    for i in range(len(merged)):
        Array[start+i] = merged[i]
        yield Array


def VisualSortAlgorithm(generator, nameOfSortAlgorithm):
    fig, ax = plt.subplots()
    array = next(generator)
    bar_rects = ax.bar(range(len(array)), array, align="center", color="red")
    
    ax.set_title(nameOfSortAlgorithm)
    ax.set_xlim(0.5, len(array)-0.5)
    ax.set_ylim(0, int(1.1 * max(array)))

    ax.set_xticks([])
    ax.set_yticks([])

    text = ax.text(0.02, 0.95, "", transform=ax.transAxes)
    iteration = [0]

    def update_fig(array, rects, iteration):
        for rect, val in zip(rects, array):
            rect.set_height(val)
        iteration[0] += 1
        text.set_text(f"Operations: {iteration[0]}")

    anim = FuncAnimation(fig, update_fig, fargs=(bar_rects, iteration), frames=generator, interval=400, repeat=False)
    plt.show()

def VisualizeAll():
    algorithms = [
        (BubbleSort, "Bubble Sort"),
        (HeapSort, "Heap Sort"),
        (InsertionSort, "Insertion Sort"),
        (MergeSort, "Merge Sort"),
        (RadixSort, "Radix Sort"),
        (SelectionSort, "Selection Sort")
    ]

    for sort_algorithm,name in algorithms:
        array = np.random.randint(10,99, size=30)
        generator = sort_algorithm(array)
        VisualSortAlgorithm(generator, name)

if __name__ == "__main__":
    VisualizeAll()