import csv
from random import Random
import multiprocessing


def generate_input(j: int):
    with open('input{}.csv'.format(j), 'w') as csvfile:
        csvfile.write("id,duration\n")
        rng = Random(j)
        for _ in range(0, 100_000_000):
            csvfile.write("{},{}\n".format(10_000_000+rng.randint(0, 9_999_999), rng.randint(1, 10000)))


def generate_ids():
    with open('ids.csv', 'w') as csvfile:
        csvfile.write("id\n")
        ids = set()
        rng = Random(99)
        while(len(ids) != 5_000_000):
            ids.add(10_000_000+rng.randint(0, 9_999_999))

        for id in ids:
            csvfile.write("{}\n".format(id))


if __name__ == '__main__':
    pool = multiprocessing.Pool()
    pool.map(generate_input, range(0, 10))
    generate_ids()
