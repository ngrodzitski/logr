from cpt.packager import ConanMultiPackager
import pprint

if __name__ == "__main__":
    builder = ConanMultiPackager(
        build_policy = "missing",
        archs = ["x86_64"],
        options = ["logr:spdlog_backend=True","logr:glog_backend=True","logr:log4cplus_backend=True", "logr:boostlog_backend=True"] )

    builder.add_common_builds(pure_c=False)
    pp = pprint.PrettyPrinter(indent=4)
    pp.pprint( builder.items )
    builder.run()
