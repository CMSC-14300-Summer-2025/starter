#!/usr/bin/env python3

import sys
import os
import random

DIR = os.path.dirname(os.path.abspath(__file__))

with open(os.path.join(DIR, 'family.txt')) as file:
    family = []
    for line in file:
        family.append(line.strip())

with open(os.path.join(DIR, 'names.txt')) as file:
    names = []
    for line in file:
        names.append(line.strip())

with open(os.path.join(DIR, 'towns.txt')) as file:
    towns = []
    for line in file:
        towns.append(line.strip())

with open(os.path.join(DIR, 'grades.txt')) as file:
    grades = []
    for line in file:
        grades.append(line.strip())

with open(os.path.join(DIR, 'majors.txt')) as file:
    majors = []
    for line in file:
        majors.append(line.strip())

adjustments = range(-5, 5)
ids = range(10000000)
late_days = range(3)

# N = 2_500_000
# N = 1_000_000
N = 10
if len(sys.argv) > 1:
    N = int(sys.argv[1])

for i in range(N):
    print(f"\r{i}", file=sys.stderr, end='')
    major = random.choice(majors)
    ucid = random.choice(ids)
    late = random.choice(late_days)
    adj = random.choice(adjustments)
    grade = random.choice(grades)
    town = random.choice(towns)

    print(f"1{ucid:07d}\t{late}\t{adj}\t{grade}\t{major}\t{town[:20]}")


print("", file=sys.stderr)
