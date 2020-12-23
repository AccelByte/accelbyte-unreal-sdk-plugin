#!/bin/bash
# Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
# This is licensed software from AccelByte Inc, for limitations
# and restrictions contact your company contract manager.

function main() {
  while getopts "s:v:j:r:p:" option; do
    case ${option} in
    s)
      serviceName=$(echo $OPTARG | awk -F/ '{print $1}')
      ;;
    v)
      serviceVersion=$OPTARG
      ;;
    j)
      jobURL=$OPTARG
      ;;
    r)
      reportURL=$OPTARG
      ;;
    p)
      if [[ $OPTARG == "SUCCESS" ]]; then
        result="Pass"
      else
        result="Fail"
      fi
      ;;
    esac
  done

  timestamp=$(date +%s)
  dateTime=$(date +%-m/%d/%Y)
  dateTimeNextDay=$(date +%-m/%d/%Y -d "+1 days")

  year=$(date +%Y)
  month=$(date +%-m)

  rm -rf ${serviceName} serviceDetail.json ${serviceName}/${year}/${month}.json
  mkdir ${serviceName}
  mkdir ${serviceName}/${year}

  # download s3 data
  aws s3 cp s3://report.accelbyte.io/quality/automation-metrics/serviceDetail.json serviceDetail.json
  aws s3 cp s3://report.accelbyte.io/quality/automation-metrics/${serviceName}/${year}/${month}.json ${serviceName}/${year}/${month}.json

  if test -f "${serviceName}/${year}/${month}.json"; then
    echo "${serviceName}/${year}/${month}.json already exist"
  else
    echo "creating file ${serviceName}/${year}/${month}.json"
    echo "[]" >${serviceName}/${year}/${month}.json
  fi

  index=$(jq --arg serviceName "${serviceName}" 'map(.repository == $serviceName) | index(true)' serviceDetail.json)
  echo "index: ${index}"
  jq --arg serviceName "${serviceName}" '.[] | select(.repository == $serviceName)' serviceDetail.json >detailTmp.json

  if grep -q "startDate" detailTmp.json; then
    jq --argjson index "${index}" \
      --arg endDate "${dateTimeNextDay}" \
      '.[$index] += {endDate: $endDate}' serviceDetail.json >detailTmp.json
    cat detailTmp.json >serviceDetail.json
  else
    jq --argjson index "${index}" \
      --arg startDate "${dateTime}" \
      --arg endDate "${dateTimeNextDay}" \
      '.[$index] += {startDate: $startDate, endDate: $endDate}' serviceDetail.json >detailTmp.json
    cat detailTmp.json >serviceDetail.json
  fi

  commitURL="https://bitbucket.org/accelbyte/${serviceName}/commits/$(git log -n 1 --pretty=format:'%H')"

  if [[ ${result} == "Fail" ]]; then
    failedTest='{"artifact":"'${reportURL}'"}'
    totalTestFail=1
  else
    failedTest="[]"
    totalTestFail=0
  fi

  jq --arg timestamp "${timestamp}" \
    --arg serviceVersion "${serviceVersion}" \
    --arg serviceName "${serviceName}" \
    --arg jobURL "${jobURL}" \
    --arg reportURL "${reportURL}" \
    --arg triggerBy "Automation" \
    --arg result "${result}" \
    --argjson failedTest "${failedTest}" \
    --arg totalTest "1" \
    --arg totalTestFail "${totalTestFail}" \
    --arg environment "DEMO" \
    --arg commitURL "${commitURL}" \
    '. += [
      {
        timestamp: $timestamp,
        serviceVersion: $serviceVersion,
        serviceName: $serviceName,
        jobURL: $jobURL,
        reportURL: $reportURL,
        triggerBy: $triggerBy,
        commitURL: $commitURL,
        environment: $environment,
        failedTest: $failedTest,
        totalTest: $totalTest,
        totalTestFail: $totalTestFail,
        result: $result
      }
    ]' ${serviceName}/${year}/${month}.json >tmp.json

  cat tmp.json >${serviceName}/${year}/${month}.json

  aws s3 cp ${serviceName}/${year}/${month}.json s3://report.accelbyte.io/quality/automation-metrics/${serviceName}/${year}/${month}.json
  aws s3 cp serviceDetail.json s3://report.accelbyte.io/quality/automation-metrics/serviceDetail.json
}

main "$@"
