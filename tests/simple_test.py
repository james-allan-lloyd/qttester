from hamcrest import *
import sys
import pytest
import rpyc
import subprocess
import os
import time

class ApplicationConnection(object):

    def __init__(self):
        # print(sys.path)
        env = dict(os.environ)
        env['PYTHONPATH'] = ";".join(sys.path)
        self.process = subprocess.Popen('build-Debug/bin/test-app.exe', env=env)
        time.sleep(1)
        self.c = rpyc.connect("localhost", 18861)

    def child(self, name):
        return self.c.root.application().child(name)

    def stop(self):
        self.c.close()
        self.process.wait()

@pytest.fixture
def app():
    app = ApplicationConnection()
    try:
        yield app
    finally:
        app.stop()

def test_setting_text(app):
    # print("(std) hello world!")
    # qttester.print("(qt) hello world!")
    # sys.stdout.write("direct\n")
    textEdit = app.child('main_window').child('centralWidget').child('textEdit')
    textEdit.plainText = "hello world"

    time.sleep(5)
    assert_that(textEdit.plainText, equal_to("hello world"))

    # sys.stdout.flush()
