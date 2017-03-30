#!/usr/bin/python
import argparse
import sys
import locale

def main():

    locale.setlocale(locale.LC_ALL, 'C')
    parser = argparse.ArgumentParser(description='Mimics the bash comm command.')
    parser.add_argument('-u', action='store_true')
    parser.add_argument('-1', '--suppress1', action='store_true')
    parser.add_argument('-2', '--suppress2', action='store_true')
    parser.add_argument('-3', '--suppress_both', action='store_true')
    parser.add_argument('file1')
    parser.add_argument('file2')
    args = parser.parse_args()

    if args.file1 == '-':
        file1 = sys.stdin.read()
    else:
        with open(args.file1,'r') as f:
            file1 = f.read()

    if args.file2 == '-':
        file2 = sys.stdin.read()
    else:
        with open(args.file2,'r') as f:
            file2 = f.read()

    file1 = file1.split()
    file2 = file2.split()

    output = []

    if args.u:
        for word in file1:
            if word in file2:
                if not args.suppress_both:
                    if args.suppress1:
                        if args.suppress2:
                            output.append(word + '\n')
                        else:
                            output.append('\t' + word + '\n')
                    elif args.suppress2:
                        output.append('\t' + word + '\n')
                    else:
                        output.append('\t\t' + word + '\n')
                file2.remove(word)
            else:
                if not args.suppress1:
                    output.append(word + '\n')
        for word in file2:
            if not args.suppress2:
                if args.suppress1:
                    output.append(word + '\n')
                else:
                    output.append('\t' + word + '\n')

    else:
        combined = file1 + file2
        combined.sort()
        both = []
        dup = set([x for x in combined if combined.count(x) > 1])

        for d in dup:
            while d in file1 and d in file2:
                file1.remove(d)
                file2.remove(d)
                both.append(d)

        unique1 = [item for item in file1 if item not in file2]
        unique2 = [item for item in file2 if item not in file1]

        if args.suppress1:
            unique1 = []

        if args.suppress2:
            unique2 = []

        if args.suppress_both:
            both = []

        all = unique1 + unique2 + both
        all.sort()

        for word in all:
            if word in both:
                if not unique1:
                    if not unique2:
                        output.append(word + '\n')
                    else:
                        output.append('\t' + word + '\n')
                else:
                    if not unique2:
                        output.append('\t' + word + '\n')
                    else:
                        output.append('\t\t' + word + '\n')
                both.remove(word)
            elif word in unique1:
                output.append(word + '\n')
            elif word in unique2:
                if not unique1:
                    output.append(word + '\n')
                else:
                    output.append('\t' + word + '\n')

    if output:
        output[-1] = output[-1].rstrip()
        out = ''.join(output)
        print (out)

if __name__ == "__main__":
    main()