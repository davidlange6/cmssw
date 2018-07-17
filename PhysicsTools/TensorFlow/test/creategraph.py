# -*- coding: utf-8 -*-

"""
Test script to create a simple graph for testing purposes at bin/data and save it using the
SavedModel serialization format.

https://github.com/tensorflow/tensorflow/blob/master/tensorflow/python/saved_model/README.md
"""


from builtins import range
import os
import sys
import tensorflow as tf


if len(sys.argv) >= 2:
    datadir = sys.argv[1]
else:
    thisdir = os.path.dirname(os.path.abspath(__file__))
    datadir = os.path.join(os.path.dirname(thisdir), "bin", "data")

x_ = tf.placeholder(tf.float32, [None, 10], name="input")
scale_ = tf.placeholder(tf.float32, name="scale")

W = tf.Variable(tf.ones([10, 1]))
b = tf.Variable(tf.ones([1]))
h = tf.add(tf.matmul(x_, W), b)
y = tf.multiply(h, scale_, name="output")

sess = tf.Session()
sess.run(tf.global_variables_initializer())

print(sess.run(y, feed_dict={scale_: 1.0, x_: [list(range(10))]})[0][0])

builder = tf.saved_model.builder.SavedModelBuilder(os.path.join(datadir, "simplegraph"))
builder.add_meta_graph_and_variables(sess, [tf.saved_model.tag_constants.SERVING])
builder.save()
