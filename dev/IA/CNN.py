import sys

from tensorflow.keras.layers import Convolution2D  
from tensorflow.keras.layers import MaxPooling2D  
from tensorflow.keras.layers import Flatten  
from tensorflow.keras.layers import Dense  
from tensorflow.keras.layers import Dropout  
from tensorflow.keras.models import Sequential  

from keras.preprocessing.image import ImageDataGenerator
import math
import os




def traitementDir(dir):
    """gère le probleme de /, sert a unifier pour facilité le traitement"""
    if dir[-1] == "/":
        tmp = dir[:-1]
    else:
        tmp = dir
    return tmp


dir = traitementDir(sys.argv[1]) 

path2write = sys.argv[2]

nbrHiddenLayer = 3
neuroneCoucheCacher = 128
tailleLot = 32
nbrPoolConv = 4
fctActivation = 'relu'
optimizer = 'adam'
imageSize = 150
rateDropOut = 0.3
shearRange = 0.2
zoomRange = 0.2
activationConvolution = 'relu'




if len(sys.argv) > 3:
    nbEpoque = int(sys.argv[3])  
else:
    nbEpoque = 15


if len(sys.argv) > 4:
    nbFiltre = int(sys.argv[4])
else:
    nbFiltre = 32


if len(sys.argv) > 5:
    kernelSize = int(sys.argv[5])
else:
    kernelSize = 3


if len(sys.argv) > 6:
    stridesConv = int(sys.argv[6])
else:
    stridesConv = 1


if len(sys.argv) > 7:
    stridesPool = int(sys.argv[7])
else:
    stridesPool = 2

if len(sys.argv) > 8:
    xPool = int(sys.argv[8])
else:
    xPool = 2

if len(sys.argv) > 9:
    yPool = int(sys.argv[9])
else:
    yPool = 2


allMetrics = ["acc"]
sizeMatricePool = (xPool, yPool)
folderTrain = dir + "/" + "training_set"
folderTest = dir + "/" + "test_set"

outUnit = 1
fctLoss = "binary_crossentropy"
classMode = 'binary'

def comptageTot(folderPathTrain, folderPathTest):
    """ compte le nombre total de photo """
    imageTest = 0
    imageTrain = 0
    foldersTest = os.listdir(folderPathTest)
    foldersTrain = os.listdir(folderPathTrain)
    for subFolders in foldersTrain:
        imageTrain += comptageImage(folderPathTrain + "/" + subFolders)
    for subFolders in foldersTest:
        imageTest += comptageImage(folderPathTest + "/" + subFolders)
    return (imageTrain, imageTest)


def comptageImage(pathFolder):
    folder = os.listdir(pathFolder)
    return len(folder)


nbImageTrain, nbImageTest = comptageTot(folderTrain, folderTest)


def buildModel():
    model = Sequential()


    model.add(Convolution2D(filters=nbFiltre, kernel_size=kernelSize, strides=stridesConv, input_shape=(imageSize, imageSize, 3),
                            activation=activationConvolution))

    model.add(MaxPooling2D(pool_size=sizeMatricePool, strides=stridesPool))


    nbrLayer = 1
    multiplicateur = 1
    for i in range(nbrPoolConv - 1):
        model.add(Convolution2D(filters=nbFiltre * multiplicateur, kernel_size=kernelSize, strides=stridesConv,
                                activation=activationConvolution))
        model.add(MaxPooling2D(pool_size=sizeMatricePool, strides=stridesPool))
        nbrLayer += 1
        multiplicateur = math.floor((nbrLayer / 2) + 1)


    model.add(Flatten())  

    for j in range(nbrHiddenLayer):
        model.add(Dense(units=neuroneCoucheCacher, activation=fctActivation))

    ##Couche de sortie
    model.add(Dropout(rateDropOut))
    model.add(Dense(units=outUnit, activation="sigmoid"))

    model.compile(optimizer=optimizer, loss=fctLoss, metrics=allMetrics)

    return model



CNN = buildModel()


train_datagen = ImageDataGenerator(
    rescale=1. / 255,
    shear_range=shearRange,
    zoom_range=zoomRange,
    horizontal_flip=True,
)


test_datagen = ImageDataGenerator(rescale=1. / 255)

trainSet = train_datagen.flow_from_directory(
    folderTrain,
    target_size=(imageSize, imageSize),
    batch_size=tailleLot,
    class_mode=classMode)

testSet = test_datagen.flow_from_directory(
    folderTest,
    target_size=(imageSize, imageSize),
    batch_size=tailleLot,
    class_mode=classMode)


saves = CNN.fit(
    trainSet,
    steps_per_epoch=math.ceil(nbImageTrain / tailleLot),
    epochs=nbEpoque,
    validation_data=testSet,
    validation_steps=math.ceil(nbImageTest / tailleLot),
    verbose=1)


def writeIntoFile(lstValTest, lstValTrain, path2write):
    f = open(path2write, "a")
    toWrite = "Epoch,ACC_Test,ACC_Train,Diff_ACC \n"
    f.write(toWrite)
    for i in range(len(lstValTest)):
        precisionTest = lstValTest[i]
        precisionEntrainement = lstValTrain[i]
        toWrite = str(i+1) +","+str(round(precisionTest,4))+ ","+str(round(precisionEntrainement,4))+ "," + str(round(abs(precisionTest-precisionEntrainement),4)) +  "\n"
        f.write(toWrite)
    f.close()

writeIntoFile(saves.history['val_acc'],saves.history['acc'],path2write)
