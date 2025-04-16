import math

def nb_derangements(n: int) -> int:
    """
    Compute the number of derangements (permutations with no fixed points) of n elements.
    The formula for the number of derangements is:
    !n = n! * sum((-1)^k / k!, k=0..n)
    """
    if n == 0:
        return 1
    elif n == 1:
        return 0
    else:
        return (n - 1) * (nb_derangements(n - 1) + nb_derangements(n - 2))

def nb_derangements_approx(n: int) -> int:
    """
    Compute the number of derangements (permutations with no fixed points) of n elements.
    The formula for the number of derangements is:
    !n = n! * sum((-1)^k / k!, k=0..n)
    This is an approximation using the formula:
    !n ~ n! / e
    """
    return math.floor(math.factorial(n - 1) / math.e)

def nb_approx_nhswo(n: int) -> int:
    return math.floor(math.factorial(n - 1) * math.e)

def nb_held_karp_subproblems_without_ordering(n: int) -> int:
    """
    Compute the theorical number of subproblems in the Held-Karp DP algorithm
    when the set of visited nodes is not ordered.
    Let n be the number of nodes in the graph, with 1 depot and n-1 customers.
    The number of subproblems is given by the formula:
    (n - 1) * 2^(n - 2) 
    Note that this doesn't count the *origin* and *destination* subproblems.
    """
    return (n - 1) * (2 ** (n - 2))

def nb_calls_held_karp_without_ordering(n: int) -> int:
    """
    Used to compare with C++ implementation, where we count the initial
    function call to the Held-Karp algorithm, corresponding to the
    *origin* subproblem.
    """
    return nb_held_karp_subproblems_without_ordering(n) + 1

def nb_held_karp_subproblems_with_ordering(n: int) -> int:
    """
    Compute the theorical number of subproblems in the Held-Karp DP algorithm
    for the TSP with n nodes, 1 depot and n-1 customers.
    This formula ignores the *origin* and *destination* subproblems,
    aka star and end states of the DP state-transition graph.
    
    Compute: (n - 1)! * \\sum_{k=0}^{n-2} 1/k!
    """
    return math.factorial(n - 1) * sum(1 / math.factorial(k) for k in range(n - 1))

def nb_calls_held_karp_with_ordering(n: int) -> int:
    """
    Used to compare with C++ implementation, where we count the initial
    function call to the Held-Karp algorithm, corresponding to the
    *origin* subproblem.
    """
    return nb_held_karp_subproblems_with_ordering(n) + 1


def main():
    for i in range(1, 20):
        print(f"n = {i}:", end=" ")
        print(f"nb Held-Karp subproblems (ordering, counting *origin*) = {math.floor(nb_calls_held_karp_with_ordering(i))}", end=" ")
        # print(f"nb Held-Karp subproblems (no ordering, counting *origin*) = {nb_calls_held_karp_without_ordering(i)}", end=" ")
        print(f"nb approx. derangements = {nb_approx_nhswo(i)}", end=" ")
        print()

if __name__ == "__main__":
    main()
