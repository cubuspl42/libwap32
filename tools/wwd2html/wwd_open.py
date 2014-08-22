import argparse
import pywap32

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('input', help='WWD file to open')
    args = parser.parse_args()
    print(args.input)
    try:
        wwd = pywap32.Wwd.open(args.input)
    except ValueError:
        print("ValueError:", args.input)