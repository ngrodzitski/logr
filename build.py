from cpt.packager import ConanMultiPackager


if __name__ == "__main__":
    builder = ConanMultiPackager(
        archs = ["x86_64"],
        options=["logr:spdlog_backend=True","logr:glog_backend=True","logr:log4cplus_backend=True"] )

    builder.add_common_builds( pure_c=False )
    builder.remove_build_if(
        lambda b: b.settings["compiler.libcxx"] != "libstdc++11" )
    builder.run()
