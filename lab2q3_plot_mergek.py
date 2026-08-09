import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

dfk = pd.read_csv("results_vary_k.csv")
dfn = pd.read_csv("results_vary_n.csv")

fig, axes = plt.subplots(2, 2, figsize=(14, 11))

# ---------------- Row 1: vary k (n fixed) ----------------

# (0,0) time vs k, log-log
ax = axes[0, 0]
ax.plot(dfk["k"], dfk["time_method1_sec"], marker="o", label="Method 1 (sequential)", linewidth=2)
ax.plot(dfk["k"], dfk["time_method2_sec"], marker="s", label="Method 2 (balanced)", linewidth=2)
ax.set_xscale("log", base=2)
ax.set_yscale("log")
ax.set_xlabel("k (number of arrays), n fixed = 500")
ax.set_ylabel("Running time (seconds, log scale)")
ax.set_title("Running Time vs k (log-log)")
ax.legend()
ax.grid(True, which="both", alpha=0.3)

# (0,1) comparisons vs k with reference curves, log-log
ax = axes[0, 1]
ax.plot(dfk["k"], dfk["comparisons_method1"], marker="o", label="Method 1 comparisons", linewidth=2)
ax.plot(dfk["k"], dfk["comparisons_method2"], marker="s", label="Method 2 comparisons", linewidth=2)

# scale references to match the last data point of each method
scale1 = dfk["comparisons_method1"].iloc[-1] / dfk["k2_ref"].iloc[-1]
scale2 = dfk["comparisons_method2"].iloc[-1] / dfk["klogk_ref"].iloc[-1]
ax.plot(dfk["k"], dfk["k2_ref"] * scale1, "--", color="tab:blue", alpha=0.6, label=r"$c_1 \cdot n k^2$ ref")
ax.plot(dfk["k"], dfk["klogk_ref"] * scale2, "--", color="tab:orange", alpha=0.6, label=r"$c_2 \cdot n k\log k$ ref")

ax.set_xscale("log", base=2)
ax.set_yscale("log")
ax.set_xlabel("k (number of arrays), n fixed = 500")
ax.set_ylabel("Comparisons (log scale)")
ax.set_title(r"Comparisons vs k: Method1 $\sim n k^2$,  Method2 $\sim n k\log k$")
ax.legend()
ax.grid(True, which="both", alpha=0.3)

# ---------------- Row 2: vary n (k fixed) ----------------

# (1,0) time vs n
ax = axes[1, 0]
ax.plot(dfn["n"], dfn["time_method1_sec"], marker="o", label="Method 1 (sequential)", linewidth=2)
ax.plot(dfn["n"], dfn["time_method2_sec"], marker="s", label="Method 2 (balanced)", linewidth=2)
ax.set_xlabel("n (size of each array), k fixed = 64")
ax.set_ylabel("Running time (seconds)")
ax.set_title("Running Time vs n (both should be linear in n)")
ax.legend()
ax.grid(True, alpha=0.3)

# (1,1) comparisons/n ratio vs n  -> should be roughly constant (confirms linear-in-n)
ax = axes[1, 1]
ratio1 = dfn["comparisons_method1"] / dfn["n"]
ratio2 = dfn["comparisons_method2"] / dfn["n"]
ax.plot(dfn["n"], ratio1, marker="o", label="Method 1: comparisons / n", linewidth=2)
ax.plot(dfn["n"], ratio2, marker="s", label="Method 2: comparisons / n", linewidth=2)
ax.set_xlabel("n (size of each array), k fixed = 64")
ax.set_ylabel("comparisons / n  (should be ~constant)")
ax.set_title("Confirms Linear Growth in n (flat lines)")
ax.legend()
ax.grid(True, alpha=0.3)
ax.set_ylim(bottom=0)

plt.tight_layout()
plt.savefig("mergek_comparison.png", dpi=150)
print("Saved plot to mergek_comparison.png")

# ---------------- Sanity-check printouts ----------------
print("\n--- Experiment A (vary k): comparisons / (n*k^2) should be ~constant for Method 1 ---")
dfk["ratio_m1_k2"] = dfk["comparisons_method1"] / (dfk["n"] * dfk["k"] ** 2)
dfk["ratio_m2_klogk"] = dfk["comparisons_method2"] / (dfk["n"] * dfk["k"] * np.log2(dfk["k"].replace(1, 2)))
print(dfk[["k", "ratio_m1_k2", "ratio_m2_klogk"]].to_string(index=False))

print("\n--- Experiment B (vary n): comparisons / n should be ~constant for both ---")
dfn["ratio_m1_n"] = dfn["comparisons_method1"] / dfn["n"]
dfn["ratio_m2_n"] = dfn["comparisons_method2"] / dfn["n"]
print(dfn[["n", "ratio_m1_n", "ratio_m2_n"]].to_string(index=False))
