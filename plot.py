import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
import os
import sys
try:
    from tqdm import tqdm
except:
    def tqdm(x):
        return x
# print "This is the name of the script: ", sys.argv[0]

def main(argv):
    try:
        directory = argv[0]
    except:
        directory = "output/"
    if (directory[-1] != '/'):
        directory += '/'
    if not os.path.exists(directory):
        print ('ERROR: Output directory not found.')
        raise

    try:
        saveto = argv[1]
    except:
        saveto = "images/"
    if (saveto[-1] != '/'):
        saveto += '/'
    if not os.path.exists(saveto):
        os.makedirs(saveto)

    files = os.listdir(directory)
    files.sort()

    for file in files:
        if not ('_phi_' in file):
            with open(directory + file, 'r') as f:
                content = f.read()
                content_ = content
                content = content[content.index('picture plane') : len(content)]
                a1_min = int(content[content.index('[') + 1 : content.index(',')])
                content = content[content.index(',') + 1 :]
                a1_max = int(content[:content.index(']')])
                content = content[content.index(']') + 1:]
                content = content[content.index('[') + 1 : content.index(']')]
                a2_min = int(content[:content.index(',')])
                a2_max = int(content[content.index(',') + 1:])
                content = content_[content_.index('step='):]
                content = content[content.index('=') + 1:]
                da = int(content[: content.index('\n')])

    a1_n = int((a1_max - a1_min) / da) + 1
    a2_n = int((a2_max - a2_min) / da) + 1
    Imax = 0
    for file in tqdm(files):
        if file.endswith(".dat") and '_' in file:
            ind = file.index('phi_')
            phi = float(file[ind+4:len(file)-4])
            dat = np.loadtxt(directory + file, delimiter=',')
            sz = int(np.sqrt(dat.shape[0]))
            dat = dat.reshape(a1_n, a2_n, 3)
            imax = dat[:,:,2].max()
            Imax = np.max([Imax, imax]);
    for file in files:
        if file.endswith(".dat") and '_' in file:
            ind = file.index('phi_')
            phi = float(file[ind+4:len(file)-4])
            dat = np.loadtxt(directory + file, delimiter=',')
            dat = dat.reshape(a1_n, a2_n, 3)

            fig = plt.figure(figsize=(6, 6))
            ax = plt.subplot(111)
            ax.set_aspect(1)
            ax.imshow(dat[:,:,2] / Imax,
                      origin='lower',
                      extent=(a2_min, a2_max, a1_min, a1_max), vmin=0, vmax=1)
            ax.set_title(r'$\phi={{{}}}^{{\circ}}$'.format(phi))
            ax.set_xlabel(r'$a_2 / R_{\rm star}$')
            ax.set_ylabel(r'$a_1 / R_{\rm star}$')
            plt.savefig(saveto + file[:-4] + '.png')
            plt.close();

if __name__ == '__main__':
    main(sys.argv[1:])
