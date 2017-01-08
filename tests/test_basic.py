from .context import gputool

import unittest


class BasicTestSuite(unittest.TestCase):
    """Basic test cases."""

    def test_return(self):
        assert gputool.main() == 0


if __name__ == '__main__':
    unittest.main()
