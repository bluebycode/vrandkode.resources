loss: 0.9965 - acc: 0.6531
```
all_histories = []
VERBOSE = 1
class MyNet:
    @staticmethod
    def build(input_shape=INPUT_SHAPE, classes=NB_CLASSES):
        model = Sequential()
        for n, filters in enumerate([32,64,128]):
            for m in np.arange(0,2):
                if (n==0 and m==0):
                    model.add(Conv2D(filters, kernel_size=3, input_shape=INPUT_SHAPE, strides=(1, 1), padding="same"))
                else:
                    model.add(Conv2D(filters, kernel_size=3, strides=(1, 1), padding="same"))

                model.add(Activation("relu"))
            model.add(MaxPooling2D(pool_size=(2, 2), strides =(2,2)))
        
        model.add(Flatten())
        for n, nodes in enumerate([]):
            model.add(Dense(layer))
            model.add(Activation("relu"))
        model.add(Dense(NB_CLASSES, activation="softmax"))
        model.summary()
        return model
    @staticmethod
    def run(model, X_train, Y_train, X_test, Y_test,epochs=20,verbose=VERBOSE):
        model.compile(loss='categorical_crossentropy', optimizer=SGD(), metrics=['accuracy'])
        history = model.fit(X_train, Y_train,
            batch_size=128, epochs=epochs,
            verbose=VERBOSE, validation_split=0.2)
        loss,acc = model.evaluate(X_test, Y_test, verbose=verbose)
        all_histories.append([acc, loss])
        print("acc:%0.3f\tloss:%0.3f\t" % (acc,loss)) 
        print("Test score:", loss)
        print('Test accuracy:', acc)
        
    @staticmethod
    def runGenerator(model, datagen, X_train, Y_train, X_test, Y_test,epochs=20,verbose=VERBOSE):
        datagen.fit(X_train)
        print("compiling")
        model.compile(loss='categorical_crossentropy', optimizer=SGD(), metrics=['accuracy'])
        print("fit")
        history = model.fit_generator(datagen.flow(X_train, Y_train,
            batch_size=128), epochs=epochs)
        loss,acc = model.evaluate(X_test, Y_test, verbose=verbose)
        all_histories.append([acc, loss])
        print("acc:%0.3f\tloss:%0.3f\t" % (acc,loss)) 
        
        
        
        from keras.preprocessing.image import ImageDataGenerator
MyNet.runGenerator(MyNet.build(), 
                ImageDataGenerator( 
                    rotation_range=90, 
                    width_shift_range=0.1, 
                    height_shift_range=0.1, 
                 horizontal_flip=True), X_train, Y_train, X_test, Y_test, epochs=200)
Epoch 72/200
391/391 [==============================] - 16s 40ms/step - loss: 0.9994 - acc: 0.6512
Epoch 73/200
 15/391 [>.............................] - ETA: 17s - loss: 0.9965 - acc: 0.6531
---------------------------------------------------------------------------
```
