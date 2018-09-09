import qttester
import sys

def test_setting_text(app):
    # print("(std) hello world!")
    # qttester.print("(qt) hello world!")
    # sys.stdout.write("direct\n")
    app.child('main_window').child('centralWidget').child('textEdit').plainText = "hello world"

    sys.stdout.flush()
