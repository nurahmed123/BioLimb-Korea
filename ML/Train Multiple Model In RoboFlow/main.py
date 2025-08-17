from roboflow import Roboflow
import os
import json

rf = Roboflow(api_key="oJWfEWOcC5WHKGQsqT3a")

workspace = rf.workspace("06nurahmed")

# replace with the IDs of your projects
# You can retrieve the project IDs from the Roboflow Dashboard by
# taking the last part of each project URL
# (i.e. https://app.roboflow.com/test/123) would have the project ID "123"
# in the workspace "test"

projects = ["bud-detection-culwf-yku1s", "mouth-yi1pl-mqzaw"]

def generate_and_train(project: str, configuration: dict) -> None:
    """
    Generate a version of a model and commence training for the new version.
    """
    rf_project = workspace.project(project)

    version_number = rf_project.generate_version(configuration)

    project_item = workspace.project(project).version(version_number)

    project_item.train()

def apply_multiple_experiments(project: str) -> None:
    """
    For each configuration in the "configurations" folder,
    generate and train a modelfor the specified project.
    """
    for configuration in os.listdir("configurations"):
        with open(f"configurations/{configuration}") as f:
            configuration = json.load(f)

        generate_and_train(project, configuration)

for project in projects:
    apply_multiple_experiments(project)