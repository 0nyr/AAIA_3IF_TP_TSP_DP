
def sum_n_bits_all_1(n: int) -> int:
    sum = 0
    for i in range(0, n):
        sum += 2 ** i
    return sum

def formula_sum_n_bits_all_1(n: int) -> int:
    """
    The formula for the sum of n bits all set to 1 is:
    sum = 2^n - 1
    """
    return (2 ** n) - 1

def main():
    for i in range(1, 20):
        print(f"n = {i}:", end=" ")
        print(f"sum_n_bits_all_1 = {sum_n_bits_all_1(i)}", end=" ")
        print(f"formula_sum_n_bits_all_1 = {formula_sum_n_bits_all_1(i)}", end=" ")
        print()

if __name__ == "__main__":
    main()