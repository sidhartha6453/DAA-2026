import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

df = pd.read_csv("results.csv")

fig, axes = plt.subplots(1, 2, figsize=(14, 5.5))

# ---------- Plot 1: Running time vs n ----------
ax = axes[0]
ax.plot(df["n"], df["time_2way_sec"], marker="o", label="Standard merge sort (2-way)", linewidth=2)
ax.plot(df["n"], df["time_3way_sec"], marker="s", label="Modified merge sort (3-way)", linewidth=2)

# scaled n*log(n) reference curve (scaled to roughly match 2-way curve at last point)
ref = df["n"] * np.log2(df["n"])
scale = df["time_2way_sec"].iloc[-1] / ref.iloc[-1]
ax.plot(df["n"], ref * scale, "--", color="gray", label=r"$c \cdot n\log n$ reference")

ax.set_xlabel("Input size n")
ax.set_ylabel("Running time (seconds)")
ax.set_title("Running Time: Merge Sort vs 3-Way Merge Sort")
ax.legend()
ax.grid(True, alpha=0.3)

# ---------- Plot 2: Comparisons vs n (log-log to show order of growth) ----------
ax2 = axes[1]
ax2.plot(df["n"], df["comparisons_2way"], marker="o", label="Standard merge sort (2-way) comparisons", linewidth=2)
ax2.plot(df["n"], df["comparisons_3way"], marker="s", label="Modified merge sort (3-way) comparisons", linewidth=2)
ax2.plot(df["n"], df["nlogn_ref"], "--", color="gray", label=r"$n\log_2 n$ reference")

ax2.set_xscale("log")
ax2.set_yscale("log")
ax2.set_xlabel("Input size n (log scale)")
ax2.set_ylabel("Number of comparisons (log scale)")
ax2.set_title("Comparisons vs n (log-log): confirms Θ(n log n) growth")
ax2.legend()
ax2.grid(True, which="both", alpha=0.3)

plt.tight_layout()
plt.savefig("mergesort_comparison.png", dpi=150)
print("Saved plot to mergesort_comparison.png")

# Also print a quick growth-rate sanity check: comparisons / (n log2 n) should be ~constant
df["ratio_2way"] = df["comparisons_2way"] / df["nlogn_ref"]
df["ratio_3way"] = df["comparisons_3way"] / df["nlogn_ref"]
print(df[["n", "ratio_2way", "ratio_3way"]].to_string(index=False))
