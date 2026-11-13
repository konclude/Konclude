# A Docker image of Konclude, made from the Linux packages of the release, for linux/amd64 and
# linux/arm64. .github/workflows/build-release-with-redland.yml builds and runs it on each
# architecture as a test.
#
# The build context holds the unpacked package of each architecture in a directory named after
# it, amd64/ and arm64/, which BuildKit selects through TARGETARCH. The binary is statically
# linked except for the C library, libm and zlib, which the base image has, so the image needs
# no RUN step and an image for another architecture is assembled without emulating it.
#
# Usage, after 'docker build -t konclude -f Docker/Dockerfile <context>', with the ontologies and
# requests of the current directory under /data, the working directory, against which relative
# paths and file: IRIs such as those of the OWLlink requests in Tests/ are resolved:
#
#   docker run --rm -v "$(pwd)":/data konclude classification -i /data/ontology.owl
#   docker run --rm -p 8080:8080 konclude owllinkserver -p 8080

# the packages are built on Ubuntu 22.04 and need its glibc 2.35 or a newer one
FROM ubuntu:24.04
# Connects an image pushed to ghcr.io with this repository, whatever pushed it (the release workflow's
# push with the workflow's own token is connected automatically anyway).
LABEL org.opencontainers.image.source="https://github.com/rhausam/Konclude"
LABEL org.opencontainers.image.description="Konclude OWL 2 DL reasoner, statically linked package on Ubuntu 24.04"

ARG TARGETARCH
COPY ${TARGETARCH}/ /opt/konclude/

WORKDIR /data
EXPOSE 8080
ENTRYPOINT ["/opt/konclude/Binaries/Konclude"]
