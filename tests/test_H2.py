import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), "..", "host"))
import h2_cli as m


class TestAnalyze(unittest.TestCase):
    def test_dedup_and_stats(self):
        text = ("00:11:22:33:44:55 -45 Smartphone\n"
                "00:11:22:33:44:55 -42 Smartphone\n"
                "00:11:22:33:44:66 -62 Headphones\n")
        rows = m.analyze(text)
        by = {r["mac"]: r for r in rows}
        self.assertEqual(len(rows), 2)
        self.assertEqual(by["00:11:22:33:44:55"]["count"], 2)
        self.assertEqual(by["00:11:22:33:44:55"]["max"], -42)
        self.assertEqual(by["00:11:22:33:44:55"]["min"], -45)


if __name__ == "__main__":
    unittest.main()
