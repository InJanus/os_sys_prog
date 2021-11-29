import re

filenames = ['standard_arbiter', 'standard_rh', 'random_arbiter', 'random_rh']

def get_matches():
    all_matches = {}
    matches = []
    p = re.compile('\\d+')
    for filename in filenames:
        matches = []
        f = open(filename, 'r')
        # print(filename)
        for x in f:
            if x[0:11] == "PHILOSOPHER":
                matches = matches +  p.findall(x)
        all_matches[filename] = matches

    # print(all_matches[filenames[0]])

    for filename in range(0,len(filenames)):

        index = []
        matches = []
        
        for count in range(0,10):
            for i in range(0,10):
                if all_matches[filenames[filename]][i*2] == str(count):
                    index = index + [str(count)]
                    matches = matches + [all_matches[filenames[filename]][i*2+1]]
        print(index)
        print(matches)

            
        

    # return all_matches  

if __name__ == "__main__":
    matches = get_matches()