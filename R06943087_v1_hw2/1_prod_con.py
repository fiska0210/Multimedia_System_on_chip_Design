n_item = 0

def Producer(n):
    global n_item
    for i in range(n):
        #assert 0
        yield
        yield
        print("Put an item")
        n_item +=1

def Consumer(n):
    global n_item
    n_get = 0
    while n_get < n:
        if n_item > 0 :
            n_item = n_item - 1
            n_get = n_get + 1
        #assert 0
        yield
        print("Get an item")

def main_loop(threads):
    from itertools import zip_longest
    for dummy in zip_longest(*threads):
        print("=== clk ===")

main_loop([Producer(10), Consumer(10)])
