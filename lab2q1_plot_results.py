"""
Plot the order-of-growth curves produced by dictionary_ops.c

Usage:
    python3 plot_results.py            # reads results.csv, writes growth_plots.png
"""
import csv
from collections import defaultdict
import matplotlib.pyplot as plt

STRUCTURES = [
    "UnsortedArray", "SortedArray",
    "SinglyLinkedUnsorted", "SinglyLinkedSorted",
    "DoublyLinkedUnsorted", "DoublyLinkedSorted",
]
OPERATIONS = ["Search", "Insert", "Delete", "Max", "Min", "Predecessor", "Successor"]

# data[operation][structure] = (list_of_n, list_of_time)
data = defaultdict(lambda: defaultdict(lambda: ([], [])))

with open("results.csv") as f:
    reader = csv.DictReader(f)
    for row in reader:
        op, struct, n, t = row["operation"], row["structure"], int(row["n"]), float(row["time_seconds"])
        ns, ts = data[op][struct]
        ns.append(n)
        ts.append(t)

fig, axes = plt.subplots(4, 2, figsize=(13, 18))
axes = axes.flatten()

for i, op in enumerate(OPERATIONS):
    ax = axes[i]
    for struct in STRUCTURES:
        ns, ts = data[op][struct]
        if ns:
            ax.plot(ns, ts, marker="o", label=struct)
    ax.set_title(f"{op}: time vs n")
    ax.set_xlabel("n (number of elements)")
    ax.set_ylabel("time (seconds, avg per op)")
    ax.legend(fontsize=7)
    ax.grid(True, alpha=0.3)

# hide the unused 8th subplot
axes[7].axis("off")

plt.tight_layout()
plt.savefig("growth_plots.png", dpi=140)
print("Saved growth_plots.png")
