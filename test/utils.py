import datetime
import os


def parse_csim_output(text) -> tuple:
    "hits:15 misses:1 evictions:0"
    return tuple(
        map(
            lambda x: int(x.split(":")[1]),
            text.strip().split(" "),
        )
    )


def parse_results_file(text) -> tuple:
    "15 1 0"
    return tuple(map(int, text.strip().split(" ")))


def format_table(data):
    column_widths = [max(len(str(item)) for item in col) for col in zip(*data)]
    table_str = ""
    for i, row in enumerate(data):
        table_str += (
            " | ".join(
                f"{str(item).ljust(width)}" for item, width in zip(row, column_widths)
            )
            + "\n"
        )
        if i == 0:
            table_str += "-+-".join("-" * width for width in column_widths) + "\n"

    table_str = table_str.strip()

    return table_str


def write_current_time(filename):
    with open(filename, "w") as file:
        file.write(datetime.datetime.now().isoformat())


def read_time(filename):
    try:
        with open(filename, "r") as file:
            now = file.read().strip()
            now = datetime.datetime.fromisoformat(now)
        return now
    except Exception as e:
        return datetime.datetime.fromisoformat("1970-01-01T00:00:00")
